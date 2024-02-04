
#include <arpirobot/core/camera/LibcameraCameraDevice.hpp>
#include <algorithm>


LibcameraCameraDevice::LibcameraCameraDevice(std::string id) : id(id){
    
}

std::string LibcameraCameraDevice::getId(){
    return id;
}

std::string LibcameraCameraDevice::getName(){
    // TODO
    // auto &props = cam->properties();
    // const auto &location = props.get(libcamera::properties::Location);
    // const auto &model = props.get(libcamera::properties::Model);
    // if(!location)
    //     return "Unknown Camera";
    // switch(*location){
    // case libcamera::properties::CameraLocationBack:
    //     return "Back Camera";
    // case libcamera::properties::CameraLocationFront:
    //     return "Front Camera";
    // case libcamera::properties::CameraLocationExternal:
    // default:
    //     if(!model)
    //         return "External Camera";
    //     return "External Camera (" + *model + ")";
    // }
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