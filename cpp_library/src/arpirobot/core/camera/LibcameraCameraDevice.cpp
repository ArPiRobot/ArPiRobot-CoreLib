
#include <arpirobot/core/camera/LibcameraCameraDevice.hpp>
#include <algorithm>


LibcameraCameraDevice::LibcameraCameraDevice(std::shared_ptr<libcamera::Camera> cam) : cam(cam){
    
}

std::string LibcameraCameraDevice::getId(){
    // TODO
}

std::string LibcameraCameraDevice::getName(){
    // TODO
}

bool LibcameraCameraDevice::supportsInputFormat(InputFormat format){
    // TODO
}

std::string LibcameraCameraDevice::capturePipeline(InputFormat format){
    // TODO
}

void LibcameraCameraDevice::setControl(std::string control, std::string value){
    // TODO
}

std::string LibcameraCameraDevice::getControl(std::string control){
    // TODO
}

bool LibcameraCameraDevice::hasControl(std::string control){
    auto controls = supportedControls();
    return std::find(controls.begin(), controls.end(), control) != controls.end();
}

std::vector<std::string> LibcameraCameraDevice::supportedControls(){
    // TODO
}