
#include <arpirobot/core/camera/V4L2CameraDevice.hpp>
#include <algorithm>
#include <libv4l2.h>
#include <linux/v4l2-common.h>
#include <linux/videodev2.h>
#include <fcntl.h>


V4L2CameraDevice::V4L2CameraDevice(std::string id) : id(id){
    
}

std::string V4L2CameraDevice::getId(){
    return id;
}

std::string V4L2CameraDevice::getName(){
    int fd = v4l2_open(id.c_str(), O_RDWR);
    if(fd < 0)
        return "Unknown Camera";

    struct  v4l2_capability caps;
    int err = v4l2_ioctl(fd, VIDIOC_QUERYCAP, &caps);
    if(err < 0){
        return "Unknown Camera";
    }
    return std::string(reinterpret_cast<char*>(caps.card));
}

bool V4L2CameraDevice::supportsInputFormat(InputFormat format){
    // TODO
}

std::string V4L2CameraDevice::capturePipeline(InputFormat format){
    // TODO
}

void V4L2CameraDevice::setControl(std::string control, std::string value){
    // TODO
}

std::string V4L2CameraDevice::getControl(std::string control){
    // TODO
}

bool V4L2CameraDevice::hasControl(std::string control){
    auto controls = supportedControls();
    return std::find(controls.begin(), controls.end(), control) != controls.end();
}

std::vector<std::string> V4L2CameraDevice::supportedControls(){
    // TODO
}