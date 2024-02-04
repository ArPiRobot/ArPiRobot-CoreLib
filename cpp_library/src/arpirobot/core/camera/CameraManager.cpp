
#include <arpirobot/core/camera/CameraManager.hpp>
#include <arpirobot/core/log/Logger.hpp>

#include <gst/gst.h>
#include <glib.h>
#include <libv4l2.h>
#include <fcntl.h>
#include <linux/v4l2-common.h>
#include <linux/videodev2.h>
#include <libcamera/libcamera.h>

#include <iostream>

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
    if(!gst_structure_has_field_typed(cap, "width", G_TYPE_INT) || 
            !gst_structure_has_field_typed(cap, "height", G_TYPE_INT)){
        // Missing required info
        return false;
    }
    gint w, h;
    gst_structure_get_int(cap, "width", &w);
    gst_structure_get_int(cap, "height", &h);

    std::vector<CameraVideoMode::Framerate> framerates;
    if(gst_structure_has_field_typed(cap, "framerate", GST_TYPE_FRACTION)){
        gint frn, frd;
        CameraVideoMode::Framerate rate;
        gst_structure_get_fraction(cap, "framerate", &frn, &frd);
        rate.numerator = frn;
        rate.denominator = frd;
        framerates.push_back(rate);
    }else if(gst_structure_has_field_typed(cap, "framerate", GST_TYPE_LIST)){
        const GValue *frlist = gst_structure_get_value(cap, "framerate");
        unsigned int count = gst_value_list_get_size(frlist);
        for(unsigned int i = 0; i < count; ++i){
            auto fr = gst_value_list_get_value(frlist, i);
            if(g_type_check_value_holds(fr, GST_TYPE_FRACTION)){
                gint frn, frd;
                frn = gst_value_get_fraction_numerator(fr);
                frd = gst_value_get_fraction_denominator(fr);
                CameraVideoMode::Framerate rate;
                rate.numerator = frn;
                rate.denominator = frd;
                framerates.push_back(rate);
            }
        }
    }
    // Note: Don't error if no framerates specified
    // libcamera through gstreamer doesn't support framerates yet...
    
    mode.width = w;
    mode.height = h;
    mode.framerates = framerates;
    return true;
}

void CameraManager::initV4l2(){
    // Phase 1: Enumerate ids and modes using gstreamer
    std::vector<std::string> ids;
    std::vector<std::vector<CameraVideoMode>> modesList;
    Logger::logDebugFrom("CameraManager", "Enumerating v4l2 devices.");
    auto factory = gst_device_provider_factory_get_by_name("v4l2deviceprovider");
    if(factory == NULL){
        Logger::logWarningFrom("CameraManager", "Unable to find v4l2 provider! v4l2 devices will be unavailable.");
    }else{
        Logger::logDebugFrom("CameraManager", "Found v4l2 provider. Searching for devices.");
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
                                CameraVideoMode mode;
                                if(gstCapToVideoMode(mode, cap)){
                                    modes.push_back(mode);
                                }
                            }
                        }
                        gst_caps_unref(caps);
                        modesList.push_back(modes);
                    }
                }
                gst_structure_free(props);
            }
        }
        g_list_free(devices);
    }
    // gst_device_provider_stop(factory);
    
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

            // Add device to list
            devices.emplace_back(CameraDevice::CameraAPI::V4L2, id, name, modes);
        }
    }
}

void CameraManager::initLibcamera(){
    // Suppress INFO messages from libcamera
    setenv("LIBCAMERA_LOG_LEVELS", "*:WARNING", true);

    // Phase 1: Enumerate ids and modes using gstreamer
    std::vector<std::string> ids;
    std::vector<std::vector<CameraVideoMode>> modesList;
    Logger::logDebugFrom("CameraManager", "Enumerating libcamera devices.");
    auto factory = gst_device_provider_factory_get_by_name("libcameraprovider");
    if(factory == NULL){
        Logger::logWarningFrom("CameraManager", "Unable to find libcamera provider! libcamera devices will be unavailable.");
    }else{
        Logger::logDebugFrom("CameraManager", "Found libcamera provider. Searching for devices.");
        auto devices = gst_device_provider_get_devices(factory);
        for(auto element = devices; element; element = element->next){
            auto dev = (GstDevice*)element->data;
            if(gst_device_has_classes(dev, "Video/Source")){
                // This device is a camera
                auto idchars = gst_device_get_display_name(dev);
                if(idchars){
                    ids.push_back(std::string(idchars));

                    // Use caps to construct video modes
                    auto caps = gst_device_get_caps(dev);
                    std::vector<CameraVideoMode> modes = {};
                    if(caps){
                        unsigned int count = gst_caps_get_size(caps);
                        for(unsigned int i = 0; i < count; ++i){
                            auto cap = gst_caps_get_structure(caps, i);
                            CameraVideoMode mode;
                            if(gstCapToVideoMode(mode, cap)){
                                modes.push_back(mode);
                            }
                        }
                    }
                    gst_caps_unref(caps);
                    modesList.push_back(modes);
                }
            }
        }
        g_list_free(devices);
    }
    // gst_device_provider_stop(factory);
    
    // Phase 2: Extra info using libcamera (must be done using gstreamer first)
    auto cm = std::make_unique<libcamera::CameraManager>();
    cm->start();
    for(unsigned int i = 0; i < ids.size(); ++i){
        std::string id = ids[i];
        std::vector<CameraVideoMode> modes = modesList[i];
        std::string name = "Unknown Camera";
        auto cam = cm->get(id);
        if(cam != nullptr){
            auto &props = cam->properties();
            const auto &location = props.get(libcamera::properties::Location);
            const auto &model = props.get(libcamera::properties::Model);
            if(location){
                switch(*location){
                case libcamera::properties::CameraLocationBack:
                    name = "Back Camera";
                case libcamera::properties::CameraLocationFront:
                    name = "Front Camera";
                case libcamera::properties::CameraLocationExternal:
                default:
                    if(!model)
                        name = "External Camera";
                    name = "External Camera (" + *model + ")";
                }
            }
            // TODO: Controls

            // Add device to list
            devices.emplace_back(CameraDevice::CameraAPI::LIBCAMERA, id, name, modes);
        }

    }
    cm->stop();
}

void CameraManager::init(){
    if(state != State::UNINITIALIZED){
        Logger::logWarningFrom("CameraManager", "Camera manager was already initialized.");
        return;
    }
    gst_init(NULL, NULL);
    initV4l2();
    initLibcamera();
    state = State::IDLE;    
    Logger::logInfoFrom("CameraManager", "Discovered " + std::to_string(devices.size()) + " devices.");
}

std::vector<CameraDevice> CameraManager::getDevices(){
    return devices;
}
