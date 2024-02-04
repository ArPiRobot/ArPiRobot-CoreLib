
#include <arpirobot/core/camera/CameraManager.hpp>
#include <arpirobot/core/log/Logger.hpp>

#include <gst/gst.h>
#include <glib.h>

// TODO: Remove
#include <iostream>

using namespace arpirobot;

CameraManager::State CameraManager::state = CameraManager::State::UNINITIALIZED;
std::vector<CameraDevice> CameraManager::devices;


void CameraManager::initV4l2(){
    // Phase 1: Enumerate ids and modes using gstreamer
    std::vector<std::string> ids;
    std::vector<std::vector<CameraVideoMode>> modes;
    gst_init(NULL, NULL);
    Logger::logDebugFrom("CameraManager", "Enumerating v4l2 devices.");
    auto factory = gst_device_provider_factory_get_by_name("v4l2deviceprovider");
    if(factory == NULL){
        Logger::logWarningFrom("CameraManager", "Unable to find v4l2 provider! v4l2 devices will be unavailable.");
    }else{
        Logger::logDebugFrom("CameraManager", "Found libcamera provider. Searching for devices.");
        auto devices = gst_device_provider_get_devices(factory);
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
                        ids.push_back(id);

                        // Use caps to construct video modes
                        auto caps = gst_device_get_caps(dev);
                        if(caps){
                            unsigned int count = gst_caps_get_size(caps);
                            for(unsigned int i = 0; i < count; ++i){
                                auto cap = gst_caps_get_structure(caps, i);
                                std::cout << "[" << gst_structure_get_name(cap) << "]" << std::endl;
                                unsigned int countInner = gst_structure_n_fields(cap);
                                for(unsigned int j = 0; j < countInner; j++){
                                    auto field = std::string(gst_structure_nth_field_name(cap, j));
                                    std::cout << "Field: " << field << std::endl;
                                    if(field == "format"){
                                        std::cout << gst_structure_get_string(cap, "format") << std::endl;
                                    }
                                }
                                std::cout << std::endl;
                            }
                        }else{
                            modes.push_back({});
                        }
                    }
                }
                gst_structure_free(props);
            }
        }
        g_list_free(devices);
    }
    
    // Phase 2: Extra info using libv4l2
    // TODO
}

void CameraManager::initLibcamera(){
    // Phase 1: Enumerate using gstreamer
    // TODO
    
    // Phase 2: Extra info using libv4l2
    // TODO

    /*
     auto cm = std::make_unique<libcamera::CameraManager>();
    cm->start();

    // Find libcamera devices (requires libcamera gstreamer plugin)
    std::vector<std::string> libcameraPaths;
    factory = gst_device_provider_factory_get_by_name("libcameraprovider");
    if(factory == NULL){
        Logger::logWarningFrom("CameraManager", "Unable to find libcamera provider! libcamera devices will be unavailable.");
    }else{
        Logger::logDebugFrom("CameraManager", "Found libcamera provider. Searching for devices.");
        auto devices = gst_device_provider_get_devices(factory);
        for(auto element = devices; element; element = element->next){
            auto dev = (GstDevice*)element->data;
            if(gst_device_has_classes(dev, "Video/Source")){
                auto name = gst_device_get_display_name(dev);
                
                auto cam = cm->get(std::string(name));
                std::cout << cam->id() << std::endl;
                libcameraPaths.push_back(std::string(name));
            }
        }
        g_list_free(devices);
    }
    */
}

void CameraManager::init(){
    if(state != State::UNINITIALIZED){
        Logger::logWarningFrom("CameraManager", "Camera manager was already initialized.");
        return;
    }
    initV4l2();
    initLibcamera();
    state = State::IDLE;    
    Logger::logInfoFrom("CameraManager", "Discovered " + std::to_string(devices.size()) + " devices.");
}
