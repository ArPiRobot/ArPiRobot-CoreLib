/*
 * Copyright 2024 Marcus Behel
 *
 * This file is part of ArPiRobot-CoreLib.
 * 
 * ArPiRobot-CoreLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ArPiRobot-CoreLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with ArPiRobot-CoreLib.  If not, see <https://www.gnu.org/licenses/>. 
 */

#include <arpirobot/core/camera/BaseCamera.hpp>
#include <gst/gst.h>
#include <opencv2/opencv.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <regex>
#include <stdexcept>

#include <boost/process.hpp>


using namespace arpirobot;


BaseCamera::BaseCamera(std::string id) : id(id){

}

BaseCamera::~BaseCamera(){
    close();
}

void BaseCamera::close(){
    stopStream();
}

std::string BaseCamera::getId(){
    return id;
}

bool BaseCamera::setCaptureMode(std::string mode){
    // Configuration and stream management must be mutually exclusive
    // If this config changes part way through stream start, stream will
    // be started incorrectly
    std::lock_guard<std::mutex> l(mutex);

    if(isStreaming)
        return false;
    
    std::regex e("^((raw|jpeg|h264):)?(\\d+)x(\\d+)(@(\\d+|\\d+\\/\\d+))?$");
    std::smatch m;
    if(std::regex_search(mode, m, e)){
        if(m[2] != "")
            capFormat = m[2];
        else
            capFormat = "raw";
        capWidth = m[3];
        capHeight = m[4];
        if(m[6] != "")
            capFramerate = m[6];
        else
            capFramerate = "30/1";
        if(capFramerate.find('/') == std::string::npos)
            capFramerate += "/1";
        return true;
    }else{
        // Invalid mode string
        return false;
    }
}

bool BaseCamera::setHwAccel(bool hwencode, bool hwdecode, bool hwconvert){
    /// If this config changes part way through stream start, stream will
    // be started incorrectly
    std::lock_guard<std::mutex> l(mutex);

    if(isStreaming)
        return false;
    
    this->hwencode = hwencode;
    this->hwdecode = hwdecode;
    this->hwconvert = hwconvert;

    return true;
}

void BaseCamera::setFrameCallback(std::function<void(cv::Mat)> *frameCallback){
    // No need to mutex this. This doesn't impact stream startup
    // process. It can also change during a stream since the pointer in the object
    // is accessed.
    this->frameCallback = frameCallback;
}

bool BaseCamera::startStreamH264(unsigned int port, unsigned int bitrate, 
        std::string profile, std::string level){
    std::lock_guard<std::mutex> l(mutex);
    if(isStreaming)
        return false;
    return doStartStreamH264(port, bitrate, profile, level);
}

bool BaseCamera::startStreamJpeg(unsigned int port, unsigned int quality){
    std::lock_guard<std::mutex> l(mutex);
    if(isStreaming)
        return false;
    return doStartStreamJpeg(port, quality);
}

void BaseCamera::stopStream(){
    std::lock_guard<std::mutex> l(mutex);
    if(isStreaming)
        doStopStream();
}

bool BaseCamera::doStartStreamH264(unsigned int port, unsigned int bitrate, 
                std::string profile, std::string level){
    // Note: Even if input is already h264, we still want to
    // re-encode because encoder settings may not be correct
    auto encodePipeline = getH264EncodeElement(bitrate, profile, level);
    encodePipeline += " ! h264parse config-interval=-1 ! video/x-h264,stream-format=byte-stream";
    auto pipeline = makeStandardPipeline(port, encodePipeline);
    return doStartStream(port, pipeline);
}

bool BaseCamera::doStartStreamJpeg(unsigned int port, unsigned int quality){
    // Note: Even if input is already jpeg, we still want to
    // re-encode because encoder settings may not be correct
    auto encodePipeline = getJpegEncodeElement(quality);
    encodePipeline += " ! jpegparse";
    auto pipeline = makeStandardPipeline(port, encodePipeline);
    return doStartStream(port, pipeline);
}

bool BaseCamera::doStartStream(unsigned int port, std::string pipeline){
    // Note: not taking mutex here. This will be called from another function
    // that is still holding it

    // First, start the ffmpeg process that will be used to actually publish
    // See comments in header for why ffmpeg is used instead of rtspclientsink
    // ffmpeg is communicated with via a named pipe
    if(access(ffmpegFifo.c_str(), F_OK) == 0){
        remove(ffmpegFifo.c_str());
    }
    if(mkfifo(ffmpegFifo.c_str(), 0666) != 0){
        Logger::logErrorFrom(getDeviceName(), "Failed to create ffmpeg fifo");
        return false;
    }
    chmod(ffmpegFifo.c_str(), 0666);
    std::string cmd = "ffmpeg -fflags nobuffer -flags low_delay -hide_banner -loglevel error -i " + ffmpegFifo + " -c:v copy -f rtsp rtsp://localhost:8554/" + std::to_string(port);
    Logger::logDebugFrom(getDeviceName(), "ffmpeg cmd: " + cmd);
    ffmpegProc = std::make_unique<boost::process::child>(cmd);
    if(!ffmpegProc->running()){
        Logger::logErrorFrom(getDeviceName(), "Failed to start ffmpeg process");
        remove(ffmpegFifo.c_str());
        return false;
    }

    // Start the pipeline on another thread
    // Note: Not using BaseRobot scheduler's threads
    // because those are reserved for dynamically scheduled tasks that yield by returning
    // This thread will "yield" by blocking on I/O
    Logger::logDebugFrom(getDeviceName(), "Pipeline: " + pipeline);
    cap = std::make_unique<cv::VideoCapture>();
    if(!cap->open(pipeline, cv::CAP_GSTREAMER)){
        Logger::logErrorFrom(getDeviceName(), "Failed to launch pipeline for stream.");
        cap->release();
        cap = nullptr;
        ffmpegProc->terminate();
        ffmpegProc->wait(); // wait after terminate to avoid zombie process
        ffmpegProc = nullptr;
        remove(ffmpegFifo.c_str());
        return false;
    }
    isStreaming = true;     // used by stream control functions to know if stream is running
    runStream = true;       // used in stop stream to halt thread before join
    thread = std::make_unique<std::thread>([this](){
        cv::Mat frame;
        while(runStream){
            cap->read(frame);
            if(frame.empty())
                continue;

            // Read frameCallback only once. It is not mutex protected, so it could
            // change between checking if null and calling it otherwise
            auto *cb = frameCallback;
            if(cb != nullptr)
                (*cb)(frame);
        }
    });
    return true;
}

void BaseCamera::doStopStream(){
    // Note: not taking mutex here. This will be called from another function
    // that is still holding it
    Logger::logInfoFrom(getDeviceName(), "Stopping stream.");
    runStream = false;
    cap->release();
    thread->join();
    isStreaming = false;
    thread = nullptr;
    cap = nullptr;
    ffmpegProc->terminate();
    ffmpegProc->wait(); // wait after terminate to avoid zombie process
    ffmpegProc = nullptr;
    remove(ffmpegFifo.c_str());
}

std::string BaseCamera::getOutputPipeline(unsigned int port){
    // buffers-soft-max=2 recover-policy=latest on tcpserversink do two things to help with network drops / latency
    //   1. If a client looses frames, they are not re-sent. Instead the latest frame is sent
    //   2. If packets are dropped, the number of buffers used is limited to avoid excessive memory usage
    // return "tcpserversink buffers-soft-max=2 recover-policy=latest host=0.0.0.0 port=" + std::to_string(port);
    return "filesink buffer-mode=2 location=" + ffmpegFifo;
}

std::string BaseCamera::makeStandardPipeline(unsigned int port, std::string encPl){
    // Must be set before getOutputPipeline can work
    ffmpegFifo = "/tmp/to_ffmpeg_" + std::to_string(port);

    std::string capPl = getCapturePipeline();       // Note: Includes capsfilter
    std::string decPl;
    std::string convert = getVideoConvertElement();
    std::string outPl = getOutputPipeline(port);
    if(capFormat == "raw"){
        decPl = "identity";
    }else if(capFormat == "jpeg"){
        decPl = getJpegDecodeElement();
    }else if(capFormat == "h264"){
        decPl = getH264DecodeElement();
    }else{
        return "";
    }
    
    // Note: SW videoconvert needed at input because colorimetry may cause issues for HW converters
    // depending on input source (often occurs for USB webcams with v4l2convert on rpi)
    // Thus, this is always SW convert, since it can handle fixing this.
    return capPl + " ! " + decPl + " ! videoconvert ! tee name=raw " + 
            "raw. ! queue ! " + convert + " ! appsink drop=true max-buffers=1 " + 
            "raw. ! queue ! " + convert + " ! " + encPl + " ! " + outPl;

}

bool BaseCamera::gstHasElement(std::string elementName){
    if(!gst_is_initialized()){
        gst_init(NULL, NULL);
    }
    auto factory = gst_element_factory_find(elementName.c_str());
    if(factory == NULL){
        return false;
    }else{
        gst_object_unref(factory);
        return true;
    }
}

std::string BaseCamera::getVideoConvertElement(){
    if(hwconvert){
        // V4L2M2M (eg RPi)
        if(gstHasElement("v4l2convert")){
            return "v4l2convert";
        }

        // VA-API has no convert element
    }

    // Fallback to software
    return "videoconvert";
}

std::string BaseCamera::getH264EncodeElement(unsigned int bitrate, std::string profile, std::string level){
    if(hwencode){
        // V4L2M2M (eg on RPi)
        if(gstHasElement("v4l2h264enc")){
            // Note: Must be explicit with both level and profile or v4l2h264enc fails
            // Note: Adding zeros to bitrate b/c video_bitrate is in bits/sec but argument is kbits/sec
            auto pl = "v4l2h264enc extra-controls=controls,repeat_sequence_header=1,video_bitrate=" + 
                    std::to_string(bitrate) + "000;" + " ! video/x-h264";
            if(level == "") level = "4";
            pl += ",level=(string)" + level;
            if(profile != "") pl += ",profile=" + profile;
            return pl;
        }
        // VA-API
        if(gstHasElement("vaapih264enc")){
            auto pl = "vaapih264enc bitrate=" + std::to_string(bitrate) + " ! video/x-h264";
            if(level != "") pl += ",level=(string)" + level;
            if(profile != "") pl += ",profile=" + profile;
            return pl;
        }
    }

    // Fallback to software
    auto pl = "x264enc key-int-max=120 tune=zerolatency speed-preset=ultrafast bitrate=" + 
            std::to_string(bitrate) + " ! video/x-h264";
    if(level != "") pl += ",level=(string)" + level;
    if(profile != "") pl += ",profile=" + profile;
    return pl;
}

std::string BaseCamera::getJpegEncodeElement(unsigned int quality){
    if(hwencode){
        // V4L2M2M (eg on RPi)
        if(gstHasElement("v4l2jpegenc"))
            return "v4l2jpegenc extra-controls=controls,compression_quality=" + std::to_string(quality) + ";";
        
        // VA-API
        if(gstHasElement("vaapijpegenc"))
            return "vaapijpegenc quality=" + std::to_string(quality);
    }

    // Fallback to software
    return "jpegenc quality=" + std::to_string(quality);
}

std::string BaseCamera::getH264DecodeElement(){
    if(hwdecode){
        // V4L2M2M (eg on RPi)
        if(gstHasElement("v4l2h264dec"))
            return "v4l2h264dec";
        
        // VA-API
        if(gstHasElement("vaapih264dec"))
            return "vaapih264dec";
    }

    // Fallback to software
    return "openh264dec";
}

std::string BaseCamera::getJpegDecodeElement(){
    if(hwdecode){
        // V4L2M2M (eg on RPi)
        if(gstHasElement("v4l2jpegdec"))
            return "v4l2jpegdec";
        
        // VA-API
        if(gstHasElement("vaapijpegdec"))
            return "vaapijpegdec";
    }

    // Fallback to software
    return "jpegdec";
}
