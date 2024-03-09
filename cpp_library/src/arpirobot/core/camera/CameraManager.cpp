
#include <arpirobot/core/camera/CameraManager.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/robot/BaseRobot.hpp>

#include <gst/gst.h>
#include <glib.h>


#include <iostream>

using namespace arpirobot;

bool CameraManager::initialized = false;
std::vector<Camera> CameraManager::cameras;

std::unordered_map<std::string, CameraManager::Stream> CameraManager::streams;

void CameraManager::init(){
    if(initialized){
        Logger::logWarningFrom("CameraManager", "Camera manager was already initialized.");
        return;
    }
    gst_init(NULL, NULL);
    initV4l2();
    initLibcamera();
    initialized = true; 
    Logger::logInfoFrom("CameraManager", "Discovered " + std::to_string(cameras.size()) + " cameras.");
}

std::vector<Camera> CameraManager::getCameras(){
    return cameras;
}

bool CameraManager::startStreamH264(std::string streamName, 
                Camera cam, 
                std::string mode, 
                std::function<void(cv::Mat)> *frameCallback, 
                bool hwaccel,
                std::string h264profile,
                unsigned int h264bitrate){    

    // Split mode string into components
    int colonPos = mode.find(":", 0);
    int xPos = mode.find("x", colonPos);
    int atPos = mode.find("@", xPos);
    if(colonPos == std::string::npos || xPos == std::string::npos){
        Logger::logErrorFrom("CameraManager", "Failed to start stream. Invalid mode string.");
        return false;
    }
    if(atPos == std::string::npos)
        atPos = mode.length();
    std::string format = mode.substr(0, colonPos);
    std::string width = mode.substr(colonPos + 1, xPos - colonPos - 1);
    std::string height = mode.substr(xPos + 1, atPos - xPos - 1);
    std::string framerate = "";
    if(atPos != mode.length())
        framerate = mode.substr(atPos + 1, mode.length() - atPos - 1);

    // Element to open camera
    std::string input;
    if(cam.api == "libcamera"){
        input = "libcamerasrc camera-name=" + cam.id;
    }else{
        input = "v4l2src device=" + cam.id;
    }

    // Caps for input
    std::string inputCaps;
    if(format == "raw"){
        inputCaps += "video/x-raw";
    }else if(format == "jpeg"){
        inputCaps += "image/jpeg";
    }else if(format == "h264"){
        inputCaps += "video/x-h264";
    }else{
        Logger::logErrorFrom("CameraManager", "Failed to start stream. Unknown input format in mode.");
        return false;
    }
    inputCaps += ",width=" + width + ",height=" + height;
    if(framerate != "")
        inputCaps += ",framerate=" + framerate;
    

    // Decoder (as needed)
    std::string decoder;
    if(format == "jpeg"){
        decoder = getJpegDecodeElement(hwaccel);
    }else if(format == "h264"){
        decoder = getH264DecodeElement(hwaccel);
    }

    // Encoder (as needed)
    std::string encoder;
    if(format != "h264"){
        encoder = getH264EncodeElement(hwaccel, h264profile, std::to_string(h264bitrate));
    }

    // Convert element
    std::string convert = getVideoConvertElement(hwaccel);

    // Stream output
    // Note: stream-format=avc is NECESSARY for v4l2h264enc to work with rtspclientsink on Raspberry Pi
    // For some reason, that's what makes it work (that and ensuring v4l2convert is right before it in pipeline)
    std::string streamOut = "h264parse ! video/x-h264,stream-format=avc,alignment=au ! rtspclientsink latency=0 location=rtsp://localhost:8554/" + streamName;

    // Construct pipeline
    // Note: drop=true max-buffers=1 properties on appsink ensure it doesn't queue frames if application reading
    // the frames is too slow. Queueing frames could stall pipeline or use lots of memory as well as adding latency
    // for the application to receive the frames
    std::string pipeline = "";
    if(format == "raw"){
        // Scenario 1: Raw input
        // Pipeline: 
        // {input} ! {input_caps} ! videoconvert ! tee name=raw 
        // raw. ! queue ! {convert} ! appsink 
        // raw. ! queue ! {convert} ! {encoder} ! {stream_out}
        // Note: need videoconvert on input side because converters down the line may be hw accelerated
        // and hw accelerated converters usually have colorometry limitations that videoconvert does not
        pipeline += input + " ! " + inputCaps + " ! videoconvert ! tee name=raw ";
        pipeline += "raw. ! queue ! " + convert + " ! appsink drop=true max-buffers=1 ";
        pipeline += "raw. ! queue ! " + convert + " ! " + encoder + " ! " + streamOut;
    }else if(format == "h264"){
        // Scenario 2: Input format matches stream format
        // Pipeline:
        // {input} ! {input_caps} ! tee name=in
        // in. ! queue ! {decoder} ! {convert} ! appsink
        // in. ! queue ! {stream_out}
        pipeline += input + " ! " + inputCaps + " ! tee name=in ";
        pipeline += "in. ! queue ! " + decoder + " ! " + convert + " ! appsink drop=true max-buffers=1 ";
        pipeline += "in. ! queue ! " + streamOut;
    }else{
        // Scenario 3: Input format not raw, and does not match stream format
        // Pipeline:
        // {input} ! {input_caps} ! {decoder} ! tee name=raw
        // raw. ! queue ! {convert} ! appsink
        // raw. ! queue ! {convert} ! {encoder} ! {stream_out}
        pipeline += input + " ! " + inputCaps + " ! " + decoder + " ! tee name=raw ";
        pipeline += "raw. ! queue ! " + convert + " ! appsink drop=true max-buffers=1 ";
        pipeline += "raw. ! queue ! " + convert + " ! " + encoder + " ! " + streamOut;
    }

    return startStreamFromPipeline(streamName, pipeline, frameCallback);
}

bool CameraManager::startStreamMjpeg(std::string streamName,
                Camera cam,
                std::string mode,
                std::function<void(cv::Mat)> *frameCallback,
                bool hwaccel,
                unsigned int quality){
    
    // TODO
    return false;
}

bool CameraManager::startStreamFromPipeline(std::string streamName,
                std::string pipeline,
                std::function<void(cv::Mat)> *frameCallback){
    if(streams.find(streamName) != streams.end()){
        Logger::logErrorFrom("CameraManager", "Failed to start stream. Duplicate stream name.");
        return false;
    }

    Logger::logInfoFrom("CameraManager", "Starting stream " + streamName);
    Logger::logDebugFrom("CameraManager", "Stream " + streamName + " pipeline: " + pipeline);

    streams[streamName] = Stream();
    streams[streamName].cap = cv::VideoCapture();
    if(!streams[streamName].cap.open(pipeline, cv::CAP_GSTREAMER)){
        streams.erase(streamName);
        Logger::logErrorFrom("CameraManager", "Failed to start stream. Failed to open capture pipeline.");
        return false;
    }
    streams[streamName].run = true;
    streams[streamName].thread = std::thread([streamName, frameCallback](){
        cv::Mat frame;
        while(streams[streamName].run){
            streams[streamName].cap.read(frame);
            if(frame.empty())
                continue; // Stream is probably shutting down. If so, run flag will be false
            if(frameCallback != nullptr)
                (*frameCallback)(frame);
        }
    });
    return true;
}

void CameraManager::stopStream(std::string streamName){
    if(streams.find(streamName) == streams.end())
        return; // No such stream
    
    Logger::logInfoFrom("CameraManager", "Stopping stream " + streamName);
    
    streams[streamName].run = false;
    streams[streamName].cap.release();
    streams[streamName].thread.join();

    streams.erase(streamName);
}

void CameraManager::stopAllStreams(){
    while(streams.size() > 0){
        stopStream((*streams.begin()).first);
    }
}

bool CameraManager::gstHasElement(std::string elementName){
    auto factory = gst_element_factory_find(elementName.c_str());
    if(factory == NULL){
        return false;
    }else{
        gst_object_unref(factory);
        return true;
    }
}

std::string CameraManager::getVideoConvertElement(bool hwaccel){
    if(hwaccel){
        // V4L2M2M (eg RPi)
        if(gstHasElement("v4l2convert")){
            return "v4l2convert";
        }
    }

    // Fallback to software
    return "videoconvert";
}

std::string CameraManager::getH264EncodeElement(bool hwaccel, std::string profile, std::string bitrate){
    if(hwaccel){
        // V4L2M2M (eg on RPi)
        if(gstHasElement("v4l2h264enc")){
            // Note: Must be explicit with both level and profile or v4l2h264enc fails
            // Note: MUST use v4l2convert before this NOT videoconvert
            // otherwise it sometimes freezes when writing to rtspclientsink
            // no idea why...
            // This is on top of having to make sure stream-format=avc or it also freezes
            // There seems to be some bad interaction between v4l2h264enc (on RPi) and rtspclientsink
            // Note: Adding zeros to bitrate b/c video_bitrate is in bits/sec but argument is kbits/sec
            return "v4l2h264enc extra-controls=encode,video_bitrate=" + bitrate + "000;" + " ! video/x-h264,level=(string)4,profile=" + profile;
        }
        // VA-API
        if(gstHasElement("vaapih264enc")){
            return "vaapih264enc bitrate=" + bitrate + " ! video/x-h264,profile=" + profile;
        }
    }

    // Fallback to software
    return "x264enc tune=zerolatency speed-preset=ultrafast bitrate=" + bitrate + " ! video/x-h264,profile=" + profile;
}

std::string CameraManager::getH264DecodeElement(bool hwaccel){
    if(hwaccel){
        // V4L2M2M (eg on RPi)
        // Disabled, because untested as of now and v4l2m2m has been nothing but trouble on the rpis
        // if(gstHasElement("v4l2h264dec"))
        //     return "v4l2h264dec";
        
        // VA-API
        if(gstHasElement("vaapih264dec"))
            return "vaapih264dec";
    }

    // Fallback to software
    return "openh264dec";
}

std::string CameraManager::getJpegEncodeElement(bool hwaccel, std::string quality){
    if(hwaccel){
        // V4L2M2M (eg on RPi)
        // Note: Disabled because it seems like v4l2jpegenc and v4l2jpegdec don't actually work on rpi?
        // if(gstHasElement("v4l2jpegenc"))
        //     return "v4l2jpegenc extra-controls=compression_quality=" + quality + ";";

        // VA-API
        if(gstHasElement("vaapijpegenc"))
            return "vaapijpegenc quality=" + quality;
    }

    // Fallback to software
    return "jpegenc quality=" + quality;
}

std::string CameraManager::getJpegDecodeElement(bool hwaccel){
    if(hwaccel){
        // V4L2M2M (eg on RPi)
        // Note: Disabled because it seems like v4l2jpegenc and v4l2jpegdec don't actually work on rpi?
        // if(gstHasElement("v4l2jpegdec"))
        //     return "v4l2jpegdec";
        
        // VA-API
        if(gstHasElement("vaapijpegdec"))
            return "vaapijpegdec";
    }

    // Fallback to software
    return "jpegdec";
}

std::vector<std::string> CameraManager::gstCapToVideoModes(GstStructure *cap){
    std::string format, width, height;
    std::vector<std::string> framerates;

    auto type = std::string(gst_structure_get_name(cap));
    if(type == "image/jpeg"){
        format = "jpeg";
    }else if(type == "video/x-raw"){
        format = "raw";
    }else if(type == "video/x-h264"){
        format = "h264";
    }else{
        // Unknown video type
        return {};
    }
    if(!gst_structure_has_field_typed(cap, "width", G_TYPE_INT) || 
            !gst_structure_has_field_typed(cap, "height", G_TYPE_INT)){
        // Missing required info
        return {};
    }
    gint w, h;
    gst_structure_get_int(cap, "width", &w);
    gst_structure_get_int(cap, "height", &h);
    width = std::to_string(w);
    height = std::to_string(h);

    if(gst_structure_has_field_typed(cap, "framerate", GST_TYPE_FRACTION)){
        gint frn, frd;
        gst_structure_get_fraction(cap, "framerate", &frn, &frd);
        framerates.push_back(std::to_string(frn) + "/" + std::to_string(frd));
    }else if(gst_structure_has_field_typed(cap, "framerate", GST_TYPE_LIST)){
        const GValue *frlist = gst_structure_get_value(cap, "framerate");
        unsigned int count = gst_value_list_get_size(frlist);
        for(unsigned int i = 0; i < count; ++i){
            auto fr = gst_value_list_get_value(frlist, i);
            if(g_type_check_value_holds(fr, GST_TYPE_FRACTION)){
                gint frn, frd;
                frn = gst_value_get_fraction_numerator(fr);
                frd = gst_value_get_fraction_denominator(fr);
                framerates.push_back(std::to_string(frn) + "/" + std::to_string(frd));
            }
        }
    }
    
    // Return list of strings in following form
    // [format]:[width]x[height]@[framerate]
    // Framerate is optional (libcamera via gstreamer does not support)
    if(framerates.size() == 0){
        return {format + ":" + width + "x" + height};
    }else if(framerates.size() == 1){
        return {format + ":" + width + "x" + height + "@" + framerates[0]};
    }else{
        std::vector<std::string> modes;
        for(auto &framerate : framerates){
            modes.push_back(format + ":" + width + "x" + height + "@" + framerate);
        }
        return modes;
    }
}

void CameraManager::initV4l2(){
    Logger::logDebugFrom("CameraManager", "Enumerating v4l2 devices.");
    auto provider = gst_device_provider_factory_get_by_name("v4l2deviceprovider");
    if(provider == NULL){
        Logger::logWarningFrom("CameraManager", "Unable to find v4l2 provider! v4l2 devices will be unavailable.");
    }else{
        Logger::logDebugFrom("CameraManager", "Found v4l2 provider. Searching for devices.");
        auto devices = gst_device_provider_get_devices(provider);
        for(auto element = devices; element; element = element->next){
            auto dev = (GstDevice*)element->data;
            if(gst_device_has_classes(dev, "Video/Source")){
                // This device is a camera
                auto props = gst_device_get_properties(dev);
                if(props){
                    if(gst_structure_has_field(props, "device.path")){
                        // Camera has a /dev/video device path
                        std::string id = std::string(gst_structure_get_string(props, "device.path"));
                        Logger::logDebugFrom("CameraManager", "Discovered v4l2 device '" + id + "'");

                        // Use caps to construct video modes
                        auto caps = gst_device_get_caps(dev);
                        std::vector<std::string> modes;
                        if(caps){
                            unsigned int count = gst_caps_get_size(caps);
                            for(unsigned int i = 0; i < count; ++i){
                                auto cap = gst_caps_get_structure(caps, i);
                                std::vector<std::string> modesSubset = gstCapToVideoModes(cap);
                                modes.insert(modes.end(), modesSubset.begin(), modesSubset.end());
                            }
                        }
                        gst_caps_unref(caps);

                        // TODO: Remove duplicate modes

                        // Have all required info. Add this device
                        cameras.push_back(Camera{
                            .api = "v4l2",
                            .id = id,
                            .modes = modes
                        });
                    }
                }
                gst_structure_free(props);
            }
        }
        g_list_free(devices);
    }
}

void CameraManager::initLibcamera(){
    // Suppress INFO messages from libcamera
    setenv("LIBCAMERA_LOG_LEVELS", "*:WARNING", true);

    Logger::logDebugFrom("CameraManager", "Enumerating libcamera devices.");
    auto provider = gst_device_provider_factory_get_by_name("libcameraprovider");
    if(provider == NULL){
        Logger::logWarningFrom("CameraManager", "Unable to find libcamera provider! libcamera devices will be unavailable.");
    }else{
        Logger::logDebugFrom("CameraManager", "Found libcamera provider. Searching for devices.");
        auto devices = gst_device_provider_get_devices(provider);
        for(auto element = devices; element; element = element->next){
            auto dev = (GstDevice*)element->data;
            if(gst_device_has_classes(dev, "Video/Source")){
                // This device is a camera
                auto idchars = gst_device_get_display_name(dev);
                if(idchars){
                    std::string id = std::string(idchars);

                    // Use caps to construct video modes
                    auto caps = gst_device_get_caps(dev);
                    std::vector<std::string> modes;
                    if(caps){
                        unsigned int count = gst_caps_get_size(caps);
                        for(unsigned int i = 0; i < count; ++i){
                            auto cap = gst_caps_get_structure(caps, i);
                            std::vector<std::string> modesSubset = gstCapToVideoModes(cap);
                            modes.insert(modes.end(), modesSubset.begin(), modesSubset.end());
                        }
                    }
                    gst_caps_unref(caps);

                    // TODO: Remove duplicate modes

                    // Have all required info. Add this device
                    cameras.push_back(Camera{
                        .api = "libcamera",
                        .id = id,
                        .modes = modes
                    });
                }
            }
        }
        g_list_free(devices);
    }
}
