
#include <arpirobot/core/camera/CameraManager.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/robot/BaseRobot.hpp>

#include <gst/gst.h>
#include <glib.h>


#include <iostream>

using namespace arpirobot;

bool CameraManager::initialized = false;
bool CameraManager::canConfigure = false;
std::vector<Camera> CameraManager::cameras;
std::unordered_map<std::string, std::unique_ptr<cv::VideoCapture>> CameraManager::streamCaptures;
std::unordered_map<std::string, std::thread> CameraManager::streamThreads;
std::unordered_map<std::string, bool> CameraManager::streamRunFlags;

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

void CameraManager::init(){
    if(initialized){
        Logger::logWarningFrom("CameraManager", "Camera manager was already initialized.");
        return;
    }
    gst_init(NULL, NULL);
    initV4l2();
    initLibcamera();
    initialized = true; 
    canConfigure = true;    // Allowed to set controls after init, but before starting streams 
    Logger::logInfoFrom("CameraManager", "Discovered " + std::to_string(cameras.size()) + " cameras.");
}

std::vector<Camera> CameraManager::getCameras(){
    return cameras;
}


bool CameraManager::startStreamH264(std::string streamName, Camera cam, std::string mode, bool hwaccel){
    // TODO: Build pipeline
    std::string pipeline;
    return startStreamGemeric(streamName, pipeline);
}

bool CameraManager::startStreamMjpeg(std::string streamName, Camera cam, std::string mode, bool hwaccel){
    // TODO: Build pipeline
    std::string pipeline;
    return startStreamGemeric(streamName, pipeline);
}

bool CameraManager::stopStream(std::string streamName){
    // TODO: Build pipeline
    std::string pipeline;
    return startStreamGemeric(streamName, pipeline);
}

bool CameraManager::startStreamGemeric(std::string streamName, std::string pipeline){
    if(streamCaptures.find(streamName) != streamCaptures.end()){
        // Already a stream by this name
        Logger::logDebugFrom("CameraManager", "A stream with the name '" + streamName + "' already exists.");
        return false;
    }
    streamCaptures[streamName] = std::make_unique<cv::VideoCapture>(pipeline, cv::CAP_GSTREAMER);
    streamRunFlags[streamName] = true;

    // Use dedicated thread instead of a scheduler "Task" b/c tasks yield control
    // by returning. This thread is IO bound. Thus, it would permanently take up 
    // thread from the scheduler's thread pool
    streamThreads[streamName] = std::thread([streamName](){
        cv::Mat frame;
        auto &cap = streamCaptures[streamName];
        while(streamRunFlags[streamName]){
            // Have to call read to keep pipeline running
            cap->read(frame);
        }
    });
    return true;
}
