#include <arpirobot/arduino/device.hpp>

using namespace arpirobot;


ArduinoDevice::ArduinoDevice(bool createDevice, int deviceId) : createDevice(createDevice), deviceId(deviceId){

}
    
ArduinoDevice::~ArduinoDevice(){

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
