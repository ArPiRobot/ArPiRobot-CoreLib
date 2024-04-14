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
    if(!gst_is_initialized()){
        gst_init(NULL, NULL);
    }
}

BaseCamera::~BaseCamera(){
    stopStream();
}

std::string BaseCamera::getId(){
    return id;
}

bool BaseCamera::setCaptureMode(std::string mode){
    // Configuration and stream management must be mutually exclusive
    // If this config changes part way through stream start, stream will
    // be started incorrectly
    std::lock_guard<std::mutex> l(managementMutex);

    if(thread != nullptr)
        return false;       // Can't change while stream running
    
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
    std::lock_guard<std::mutex> l(managementMutex);

    if(thread != nullptr)
        return false; // Can't change while streaming
    
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

bool BaseCamera::startStreamH264(std::string key, unsigned int bitrate, 
        std::string profile, std::string level){
    std::lock_guard<std::mutex> l(managementMutex);
    if(thread != nullptr)
        return false;
    std::replace(key.begin(), key.end(), '/', '_');
    std::replace(key.begin(), key.end(), '\\', '_');
    return doStartStreamH264(key, bitrate, profile, level);
}

bool BaseCamera::startStreamJpeg(std::string key, unsigned int quality){
    std::lock_guard<std::mutex> l(managementMutex);
    if(thread != nullptr)
        return false;
    std::replace(key.begin(), key.end(), '/', '_');
    std::replace(key.begin(), key.end(), '\\', '_');
    return doStartStreamJpeg(key, quality);
}

void BaseCamera::stopStream(){
    std::lock_guard<std::mutex> l(managementMutex);
    if(thread != nullptr)
        doStopStream();
}

std::string BaseCamera::getOutputPipeline(std::string key){
    // buffers-soft-max=2 recover-policy=latest on tcpserversink do two things to help with network drops / latency
    //   1. If a client looses frames, they are not re-sent. Instead the latest frame is sent
    //   2. If packets are dropped, the number of buffers used is limited to avoid excessive memory usage
    // return "tcpserversink buffers-soft-max=2 recover-policy=latest host=0.0.0.0 port=" + std::to_string(port);
    return "filesink buffer-mode=2 location=/tmp/to_ffmpeg_" + key;
}

std::string BaseCamera::makeStandardPipeline(std::string key, std::string encPl){
    std::string capPl = getCapturePipeline();       // Note: Includes capsfilter
    std::string decPl;
    std::string convert = getVideoConvertElement();
    std::string outPl = getOutputPipeline(key);
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

bool BaseCamera::doStartStreamH264(std::string key, unsigned int bitrate, 
                std::string profile, std::string level){
    // Note: Even if input is already h264, we still want to
    // re-encode because encoder settings may not be correct
    auto encodePipeline = getH264EncodeElement(bitrate, profile, level);
    encodePipeline += " ! h264parse config-interval=-1 ! video/x-h264,stream-format=byte-stream";
    auto pipeline = makeStandardPipeline(key, encodePipeline);
    return doStartStream(key, pipeline);
}

bool BaseCamera::doStartStreamJpeg(std::string key, unsigned int quality){
    // Note: Even if input is already jpeg, we still want to
    // re-encode because encoder settings may not be correct
    auto encodePipeline = getJpegEncodeElement(quality);
    encodePipeline += " ! jpegparse";
    auto pipeline = makeStandardPipeline(key, encodePipeline);
    return doStartStream(key, pipeline);
}

bool BaseCamera::doStartStream(std::string key, std::string pipeline){
    shouldStreamRun = true;
    streamStartDone = false;
    streamStartSuccess = false;

    thread = std::make_unique<std::thread>(&BaseCamera::runStream, this, key, pipeline);
    std::unique_lock<std::mutex> l(streamStartMutex);
    streamStartCv.wait(l, [this](){ return streamStartDone; });
    if(!streamStartSuccess){
        Logger::logErrorFrom(getDeviceName(), "Failed to start stream");
        return false;
    }
    Logger::logInfoFrom(getDeviceName(), "Stream running");
    return true;
}

void BaseCamera::runStream(std::string key, std::string pipeline){
    std::string ffmpegFifo = "/tmp/to_ffmpeg_" + key;
    std::unique_ptr<boost::process::child> ffmpegProc;
    GstElement *gstPl = NULL;
    GstBus *gstBus = NULL;
    // TODO: Things to capture frame from pipeline using appsink

    // Setup everything for the stream
    {
        std::lock_guard<std::mutex> l(streamStartMutex); // for cond var to work properly

        streamStartSuccess = true;

        if(!extraSetup(key, pipeline)){
            streamStartSuccess = false;
        }

        // Create ffmpeg fifo
        if(streamStartSuccess){
            if(access(ffmpegFifo.c_str(), F_OK) == 0){
                remove(ffmpegFifo.c_str());
            }
            if(mkfifo(ffmpegFifo.c_str(), 0666) != 0){
                Logger::logErrorFrom(getDeviceName(), "Failed to create ffmpeg fifo");
                streamStartSuccess = false;
            }else{
                // chmod behaves differently than mode given to mkfifo does.
                // Sometimes, actual chmod is needed instead of relying on mkfifo
                chmod(ffmpegFifo.c_str(), 0666);
            }
        }

        // Setup ffmpeg (reads frames from fifo and writes to rtsp server)
        // Why using ffmpeg instead of just using gstreamer's rtspclientsink?
        // Because for some reason on rpi, v4l2m2m encoders seem to just cause pipelines to break (freeze after a second or two)
        // Everything works fine taking that same data and piping to ffmpeg though, so just do that
        if(streamStartSuccess){
            std::string cmd = "ffmpeg -fflags nobuffer -flags low_delay -hide_banner -loglevel error -i " + ffmpegFifo + " -c:v copy -f rtsp rtsp://localhost:8554/" + key;
            Logger::logDebugFrom(getDeviceName(), "ffmpeg cmd: " + cmd);
            try{
                ffmpegProc = std::make_unique<boost::process::child>(cmd);
            }catch(const boost::process::process_error &e){
                Logger::logErrorFrom(getDeviceName(), "Failed to start ffmpeg process");
                streamStartSuccess = false;
            }
        }

        // Setup and start gstreamer pipeline
        // Note: OpenCV is not used to run gstreamer pipeline because
        // VideoCapture::read  will block forever if gstreamer pipeline "stalls"
        // which can happen if v4l2m2m elements have an issue (eg linking libcamerasrc with v4l2jpegdec)
        // or if a pipe stalls (eg ffmpeg process dies first due to ctrl+c and the pipe write blocks
        // because there is no reader). In these cases, cleanly closing a cv VideoCapture
        // is difficult and requires calling release() from a different thead. This causes
        // segmentation faults when using libcamerasrc in the pipeline, and also does not seem
        // to be recommended practice for using OpenCV. As such, gstreamer is just used directly
        // to run the pipeline, and data is manually put into a cv::Mat which users could then
        // use with OpenCV as desired (using frame callback)
        if(streamStartSuccess){
            Logger::logDebugFrom(getDeviceName(), "gst pipeline: " + pipeline);
            gstPl = gst_parse_launch(pipeline.c_str(), NULL);
            auto ret = gst_element_set_state(gstPl, GST_STATE_PLAYING);
            if(ret == GST_STATE_CHANGE_FAILURE){
                Logger::logErrorFrom(getDeviceName(), "Failed to start gstreamer pipeline");
                streamStartSuccess = false;
            }else{
                gstBus = gst_element_get_bus (gstPl);
            }
        }

        streamStartDone = true; // Done var is needed in case notify_one called before other thread wait()
    }

    // Inform the stream start status
    streamStartCv.notify_one();

    // Main loop (runs the pipeline)
    while(streamStartSuccess && shouldStreamRun){
        GstMessage *msg = gst_bus_timed_pop_filtered(gstBus, 1 * GST_SECOND, 
                static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));
        GError *err;
        gchar *debug_info;
        if(msg != NULL){
            switch(GST_MESSAGE_TYPE(msg)){
            case GST_MESSAGE_ERROR:
                gst_message_parse_error (msg, &err, &debug_info);
                Logger::logErrorFrom(getDeviceName(), "Gstreamer pipeline had error");
                Logger::logErrorFrom(getDeviceName(), "Error from element " + 
                        std::string(GST_OBJECT_NAME(msg->src)) + " " + std::string(err->message));
                Logger::logDebugFrom(getDeviceName(), std::string(debug_info));
                g_clear_error (&err);
                g_free (debug_info);
                break;
            case GST_MESSAGE_EOS:
                Logger::logErrorFrom(getDeviceName(), "Gstreamer pipeline EoS");
                break;
            default:
                // This shouldn't happen because pop is filtered
                break;
            }
            gst_message_unref(msg);
        }
    }

    // Cleanup
    remove(ffmpegFifo.c_str());
    if(ffmpegProc != nullptr){
        ffmpegProc->terminate();
        ffmpegProc = nullptr;
    }
    if(gstPl != NULL){
        if(gstBus != NULL){
            gst_object_unref(gstBus);
        }
        gst_element_set_state (gstPl, GST_STATE_NULL);
        gst_object_unref(gstPl);
    }
    extraTeardown(key, pipeline);
}

bool BaseCamera::extraSetup(std::string key, std::string pipeline){
    return true;
}

void BaseCamera::extraTeardown(std::string key, std::string pipeline){

}

void BaseCamera::doStopStream(){
    shouldStreamRun = false;
    thread->join();
    thread = nullptr;
}

bool BaseCamera::gstHasElement(std::string elementName){
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
