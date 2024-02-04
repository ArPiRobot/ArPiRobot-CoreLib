
#include <arpirobot/core/camera/CameraDevice.hpp>

using namespace arpirobot;

CameraDevice::CameraDevice(CameraAPI api, 
        std::string id, std::string name, std::vector<CameraVideoMode> modes) : 
        api(api), id(id), name(name), modes(modes){

}

CameraDevice::CameraAPI CameraDevice::getApi(){
    return api;
}

std::string CameraDevice::getId(){
    return id;
}

std::string CameraDevice::getName(){
    return name;
}

std::vector<CameraVideoMode> CameraDevice::getModes(){
    return modes;
}