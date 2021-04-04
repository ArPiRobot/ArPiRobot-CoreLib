#include <arpirobot/arduino/sensor/SingleEncoder.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/conversions.hpp>

using namespace arpirobot;


SingleEncoder::SingleEncoder(int pin, bool useInternalPullup, bool createDevice, int deviceId) : 
        ArduinoDevice(createDevice, deviceId), pin(std::to_string(pin)), useInternalPullup(useInternalPullup){

}

SingleEncoder::SingleEncoder(std::string pin, bool useInternalPullup, bool createDevice, int deviceId) : 
        ArduinoDevice(createDevice, deviceId), pin(pin), useInternalPullup(useInternalPullup){

}

int SingleEncoder::getPosition(){
    return count + countOffset;
}

void SingleEncoder::setPosition(int newPosition){
    countOffset = newPosition - count;
}

std::string SingleEncoder::getDeviceName(){
    return "SingleEncoder(" + pin + ")";
}

void SingleEncoder::applyDefaultState(){
    count = 0;
    countOffset = 0;
}

std::vector<uint8_t> SingleEncoder::getCreateData(){
    uint8_t isAnalog = pin[0] == 'A' || pin[0] == 'a';
    uint8_t p;
    try{
        if(isAnalog){
            p = std::stoi(pin.substr(1, pin.length() - 1));
        }else{
            p = std::stoi(pin);
        }
    }catch(const std::runtime_error &e){
        Logger::logWarningFrom(getDeviceName(), "Invalid pin number specified.");
        return {};
    }

    // ADDSENC,is_analog,pin,pullup
    std::vector<uint8_t> data = stringToData("ADDSENC");
    data.push_back(isAnalog);
    data.push_back(p);
    data.push_back(useInternalPullup);
    return data;
}

void SingleEncoder::handleData(const std::vector<uint8_t> &data){
    // Buffer contains deviceId, data..., crc, crc
    if(data.size() >= 5){
        // At least 2 bytes of data
        count = Conversions::convertDataToInt16(data, 1, true);
    }
}
