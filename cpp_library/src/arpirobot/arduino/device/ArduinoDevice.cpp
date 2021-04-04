#include <arpirobot/arduino/device/ArduinoDevice.hpp>

using namespace arpirobot;


ArduinoDevice::ArduinoDevice(bool createDevice, int deviceId) : createDevice(createDevice), deviceId(deviceId){

}
    
ArduinoDevice::~ArduinoDevice(){

}

std::vector<uint8_t> ArduinoDevice::stringToData(const std::string &str){
    std::vector<uint8_t> data;
    data.reserve(str.length());
    for(size_t i = 0; i < str.length(); ++i){
        data.push_back((uint8_t)str[i]);
    }
    return data;
}

void ArduinoDevice::setArduino(BaseArduinoInterface *arduino){
    this->arduino = arduino;
}

void ArduinoDevice::setDeviceId(int deviceId){
    this->deviceId = deviceId;
}

bool ArduinoDevice::sendData(const std::vector<uint8_t> &data){
    if(arduino == nullptr)
        return false;
    
    // Add '-' to the front to indicate that this data is to be sent to a device
    // Then add the id of the device
    std::vector<uint8_t> fullData;
    fullData.reserve(data.size() + 2);
    fullData.push_back('-');
    fullData.push_back((uint8_t)deviceId);
    for(size_t i = 0; i < data.size(); ++i){
        fullData.push_back(data[i]);
    }
    return arduino->writeData(fullData);
}

bool ArduinoDevice::sendData(const std::string &data){
    std::vector<uint8_t> realData;
    realData.reserve(data.length());
    for(size_t i = 0; i < data.length(); ++i){
        realData.push_back(data[i]);
    }
    return sendData(realData);
}
