
#include <arpirobot/core/camera/V4L2CameraDevice.hpp>
#include <algorithm>


V4L2CameraDevice::V4L2CameraDevice(std::string id) : id(id){
    
}

std::string V4L2CameraDevice::getId(){
    // TODO
}

std::string V4L2CameraDevice::getName(){
    // TODO
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