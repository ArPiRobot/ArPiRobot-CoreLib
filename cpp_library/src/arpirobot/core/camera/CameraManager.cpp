
#include <arpirobot/core/camera/CameraManager.hpp>
#include <arpirobot/core/log/Logger.hpp>

#include <gst/gst.h>
#include <glib.h>

using namespace arpirobot;


std::vector<std::shared_ptr<LibcameraCameraDevice>> CameraManager::libcameraDevices;
std::vector<std::shared_ptr<V4L2CameraDevice>> CameraManager::v4l2Devices;

void CameraManager::init(){
    // Initialize gstreamer
    gst_init(NULL, NULL);


    // -------------------------------------------------------------------------
    // Enumerate devices using gstreamer
    // -------------------------------------------------------------------------
    libcameraDevices.clear();
    v4l2Devices.clear();

    Logger::logDebugFrom("CameraManager", "Starting camera discovery.");

    // Find v4l2 devices
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
                        auto id = std::string(gst_structure_get_string(props, "device.path"));
                        Logger::logDebugFrom("CameraManager", "Discovered v4l2 device '" + id + "'");
                        v4l2Devices.push_back(std::make_shared<V4L2CameraDevice>(id));
                    }
                }
                gst_structure_free(props);
            }
        }
        g_list_free(devices);
    }
    
    // Find libcamera devices (requires libcamera gstreamer plugin)
    factory = gst_device_provider_factory_get_by_name("libcameraprovider");
    if(factory == NULL){
        Logger::logWarningFrom("CameraManager", "Unable to find libcamera provider! libcamera devices will be unavailable.");
    }else{
        Logger::logDebugFrom("CameraManager", "Found libcamera provider. Searching for devices.");
        auto devices = gst_device_provider_get_devices(factory);
        for(auto element = devices; element; element = element->next){
            auto dev = (GstDevice*)element->data;
            if(gst_device_has_classes(dev, "Video/Source")){
                auto id = gst_device_get_display_name(dev);
                auto idStr = std::string(id);
                g_free(id);
                Logger::logDebugFrom("CameraManager", "Discovered libcamera device '" + idStr + "'");
                libcameraDevices.push_back(std::make_shared<LibcameraCameraDevice>(idStr));
            }
        }
        g_list_free(devices);
    }

    unsigned int count = libcameraDevices.size() + v4l2Devices.size();
    Logger::logInfoFrom("CameraManager", "Discovered " + std::to_string(count) + " devices.");
    // -------------------------------------------------------------------------
}

