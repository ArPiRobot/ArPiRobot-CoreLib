
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

    std::string capturePipeline = getCapturePipeline(cam, mode, hwaccel);
    if(capturePipeline == "")
        // Error message logged in getCapturePipeline function, so no need to log here
        return false;

    std::string publishPipeline = "appsrc is-live=true block=true";

    // Convert as needed (OpenCV typically uses RGB type space but encoders typically expect YUV type space)
    publishPipeline += " ! " + getVideoConvertElement(hwaccel);

    // Encode frames
    publishPipeline += " ! " + getH264EncodeElement(hwaccel, h264profile, std::to_string(h264bitrate));

    // Publish to RTSP server
    // Note: stream-format=avc is NEEDED for v4l2h264enc to work properly with rstpclientsink
    publishPipeline += " ! h264parse config_interval=-1 ! video/x-h264,stream-format=avc,alignment=au ! rtspclientsink location=rtsp://127.0.0.1:8554/" + streamName;

    return startStreamFromPipelines(streamName, capturePipeline, publishPipeline, frameCallback);
}

bool CameraManager::startStreamMjpeg(std::string streamName,
                Camera cam,
                std::string mode,
                std::function<void(cv::Mat)> *frameCallback,
                bool hwaccel,
                unsigned int quality){
    // Note: Even if input is jpeg, decode then re-encode
    // This is necessary becuase opencv needs raw (decoded) frames
    // Could do some stuff with gstreamer tee to get around this
    // but that requires more complex logic (and jpeg encoding is easy enough)

    std::string capturePipeline = getCapturePipeline(cam, mode, hwaccel);
    if(capturePipeline == "")
        // Error message logged in getCapturePipeline function, so no need to log here
        return false;

    std::string publishPipeline = "appsrc is-live=true block=true";

    // Convert as needed (OpenCV typically uses RGB type space but encoders typically expect YUV type space)
    // Note: Must add caps filter because jpeg encoder requires extra info. See https://bugzilla.gnome.org/show_bug.cgi?id=763331
    publishPipeline += " ! " + getVideoConvertElement(hwaccel) + " ! video/x-raw,format=YUY2";

    // Encode frames
    publishPipeline += " ! " + getJpegEncodeElement(hwaccel, std::to_string(quality));

    // Publish to RTSP server
    publishPipeline += " ! rtspclientsink location=rtsp://127.0.0.1:8554/" + streamName;

    return startStreamFromPipelines(streamName, capturePipeline, publishPipeline, frameCallback);
}

bool CameraManager::startStreamFromPipelines(std::string streamName,
                std::string capturePipeline,
                std::string publishPipeline,
                std::function<void(cv::Mat)> *frameCallback){
    if(streams.find(streamName) != streams.end()){
        Logger::logErrorFrom("CameraManager", "Failed to start stream. Duplicate stream name.");
        return false;
    }

    Logger::logInfoFrom("CameraManager", "Starting stream " + streamName);
    Logger::logDebugFrom("CameraManager", "Capture pipeline: " + capturePipeline);
    Logger::logDebugFrom("CameraManager", "Publish pipeline: " + publishPipeline);
    
    streams[streamName] = Stream();
    streams[streamName].cap = cv::VideoCapture();
    streams[streamName].pub = cv::VideoWriter();
    if(!streams[streamName].cap.open(capturePipeline, cv::CAP_GSTREAMER)){
        streams.erase(streamName);
        Logger::logErrorFrom("CameraManager", "Failed to start stream. Failed to open capture pipeline.");
        return false;
    }
    double width = (double)streams[streamName].cap.get(cv::CAP_PROP_FRAME_WIDTH);
    double height = (double)streams[streamName].cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    double fps = (double)streams[streamName].cap.get(cv::CAP_PROP_FPS);
    cv::Size frameSize(width, height);
    if(!streams[streamName].pub.open(publishPipeline, cv::CAP_GSTREAMER, 0, fps, frameSize)){
        streams.erase(streamName);
        Logger::logErrorFrom("CameraManager", "Failed to start stream. Failed to open publish pipeline.");
        return false;
    }
    streams[streamName].run = true;
    streams[streamName].thread = std::thread([streamName, frameCallback](){
        cv::Mat frame;
        while(streams[streamName].run){
            streams[streamName].cap.read(frame);
            if(frame.empty())
                continue; // Stream is probably shutting down. If so, run flag will be false
            if(!streams[streamName].run)
                break; // Shutting down, don't try to write or it could cause an issue when writer is released
            streams[streamName].pub.write(frame);
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
    streams[streamName].pub.release();
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

std::vector<std::string> CameraManager::gstCapToVideoModes(GstStructure *cap){
    std::string format, width, height;
    std::vector<std::string> framerates;

    auto type = std::string(gst_structure_get_name(cap));
    if(type == "image/jpeg"){
        format = "mjpeg";
    }else if(type == "video/x-raw"){
        if(!gst_structure_has_field(cap, "format")){
            // Unknown input format
            return {};
        }
        auto fmtField = std::string(gst_structure_get_string(cap, "format"));
        if(fmtField == "YUY2"){
            format = "yuy2";
        }else{
            // Unsupported raw format
            return {};
        }
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

std::string CameraManager::getCapturePipeline(Camera cam, std::string mode, bool hwaccel){
    // Open camera
    std::string pipeline;
    if(cam.api == "libcamera"){
        pipeline = "libcamerasrc camera-name=" + cam.id;
    }else{
        pipeline = "v4l2src device=" + cam.id;
    }

    // Caps for grabbing frames from camera
    // mode = format:widthxheight@framerate (@framerate is optional)
    int colonPos = mode.find(":", 0);
    int xPos = mode.find("x", colonPos);
    int atPos = mode.find("@", xPos);
    if(atPos == std::string::npos)
        atPos = mode.length();
    std::string width = mode.substr(colonPos + 1, xPos - colonPos - 1);
    std::string height = mode.substr(xPos + 1, atPos - xPos - 1);
    std::string framerate = "";
    if(atPos != mode.length())
        framerate = mode.substr(atPos + 1, mode.length() - atPos - 1);
    if(mode.find("mjpeg", 0) == 0){
        pipeline += " ! image/jpeg";
    }else if(mode.find("yuy2", 0) == 0){
        pipeline += " ! video/x-raw,format=YUY2";
    }else{
        Logger::logErrorFrom("CameraManager", "Failed to start stream. Unknown input format in mode.");
        return "";
    }
    pipeline += ",width=" + width;
    pipeline += ",height=" + height;
    if(framerate != "")
        pipeline += ",framerate=" + framerate;

    // Decode input as needed
    if(mode.find("mjpeg", 0) == 0){
        pipeline += " ! " + getJpegDecodeElement(hwaccel);
    }

    // Convert input format as needed
    pipeline += " ! " + getVideoConvertElement(hwaccel);
    
    // Output to app sink
    pipeline += " ! appsink";

    return pipeline;
}

std::string CameraManager::getVideoConvertElement(bool hwaccel){
    if(hwaccel){
        // OMX (eg RPi)
        if(gstHasElement("v4l2convert")){
            return "v4l2convert";
        }

        // TODO: VA-API?
    }

    // Fallback to software
    return "videoconvert";
}

std::string CameraManager::getH264EncodeElement(bool hwaccel, std::string profile, std::string bitrate){
    if(hwaccel){
        // OMX (eg on RPi)
        if(gstHasElement("v4l2h264enc")){
            // Note: Must be explicit with both level and profile or v4l2h264enc fails
            return "v4l2h264enc extra-controls=encode,video_bitrate=" + bitrate + ";" + " ! video/x-h264,level=(string)4,profile=" + profile;
        }
        // VA-API
        if(gstHasElement("vaapih264enc")){
            return "vaapih264enc bitrate=" + bitrate + " ! video/x-h264,profile=" + profile;
        }
    }

    // Fallback to software
    return "x264enc tune=zerolatency speed-preset=ultrafast bitrate=" + bitrate + " ! video/x-h264,profile=" + profile;
}

std::string CameraManager::getJpegEncodeElement(bool hwaccel, std::string quality){
    if(hwaccel){
        // OMX (eg on RPi)
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
        // OMX (eg on RPi)
        // if(gstHasElement("v4l2jpegdec"))
        //     return "v4l2jpegdec";
        
        // VA-API
        if(gstHasElement("vaapijpegdec"))
            return "vaapijpegdec";
    }

    // Fallback to software
    return "jpegdec";
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
