
#include <arpirobot/core/camera/CameraManager.hpp>
#include <arpirobot/core/log/Logger.hpp>

#include <gst/gst.h>
#include <glib.h>
#include <libv4l2.h>
#include <fcntl.h>
#include <linux/v4l2-common.h>
#include <linux/videodev2.h>


using namespace arpirobot;

CameraManager::State CameraManager::state = CameraManager::State::UNINITIALIZED;
std::vector<CameraDevice> CameraManager::devices;

bool CameraManager::gstCapToVideoMode(CameraVideoMode &mode, GstStructure *cap){
    auto type = std::string(gst_structure_get_name(cap));
    if(type == "image/jpeg"){
        mode.format = CameraVideoMode::Format::MJPEG;
    }else if(type == "video/x-raw"){
        if(!gst_structure_has_field(cap, "format")){
            // Unknown input format
            return false;
        }
        auto format = std::string(gst_structure_get_string(cap, "format"));
        if(format == "YUY2"){
            mode.format = CameraVideoMode::Format::YUY2;
        }else{
            // Unsupported raw format
            return false;
        }
    }
    if(!gst_structure_has_field(cap, "width") || 
            !gst_structure_has_field(cap, "height") || 
            !gst_structure_has_field(cap, "framerate")){
        // Missing required info
        return false;
    }
    gint w, h;
    bool res = gst_structure_get_int(cap, "width", &w);
    res = res && gst_structure_get_int(cap, "height", &h);
    gint frn, frd;
    res = res && gst_structure_get_fraction(cap, "framerate", &frn, &frd);
    if(res){
        mode.width = w;
        mode.height = h;
        mode.framerateNumerator = frn;
        mode.framerateDenominator = frd;
        return true;
    }else{
        // Required info is unexpected type?
        return false;
    }
}

void CameraManager::initV4l2(){
    // Phase 1: Enumerate ids and modes using gstreamer
    std::vector<std::string> ids;
    std::vector<std::vector<CameraVideoMode>> modesList;
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
                        std::vector<CameraVideoMode> modes = {};
                        if(caps){
                            unsigned int count = gst_caps_get_size(caps);
                            for(unsigned int i = 0; i < count; ++i){
                                auto cap = gst_caps_get_structure(caps, i);
                                gst_structure_fixate(cap);
                                CameraVideoMode mode;
                                if(gstCapToVideoMode(mode, cap)){
                                    modes.push_back(mode);
                                }
                            }
                        }
                        modesList.push_back(modes);
                    }
                }
                gst_structure_free(props);
            }
        }
        g_list_free(devices);
    }
    gst_deinit();
    
    // Phase 2: Extra info using libv4l2 (must be done using gstreamer first)
    for(unsigned int i = 0; i < ids.size(); ++i){
        std::string id = ids[i];
        std::vector<CameraVideoMode> modes = modesList[i];
        std::string name = "Unknown Camera";

        int fd = v4l2_open(id.c_str(), O_RDWR);
        if(fd >= 0){
            struct  v4l2_capability v4l2caps;
            int err = v4l2_ioctl(fd, VIDIOC_QUERYCAP, &v4l2caps);
            if(err >= 0){
                name = std::string(reinterpret_cast<char*>(v4l2caps.card));
            }
            // TODO: Controls
            v4l2_close(fd);
        }

        // Add device to list
        devices.emplace_back(CameraAPI::V4L2, id, name, modes);
    }
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

std::vector<CameraDevice> CameraManager::getDevices(){
    return devices;
}
