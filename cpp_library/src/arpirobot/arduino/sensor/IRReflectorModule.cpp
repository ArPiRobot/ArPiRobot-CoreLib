#include <arpirobot/arduino/sensor/IRReflectorModule.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/conversions.hpp>

using namespace arpirobot;


IRReflectorModule::IRReflectorModule(int digitalPin, bool createDevice, int deviceId) : 
        ArduinoDevice(createDevice, deviceId), digitalPin(std::to_string(digitalPin)), analogPin(""){

}

IRReflectorModule::IRReflectorModule(std::string digitalPin, bool createDevice, int deviceId) : 
        ArduinoDevice(createDevice, deviceId), digitalPin(digitalPin), analogPin(""){

}

IRReflectorModule::IRReflectorModule(int digitalPin, std::string analogPin, bool createDevice, int deviceId) : 
        ArduinoDevice(createDevice, deviceId), digitalPin(std::to_string(digitalPin)), analogPin(analogPin){

}

IRReflectorModule::IRReflectorModule(std::string digitalPin, std::string analogPin, bool createDevice, int deviceId)  : 
        ArduinoDevice(createDevice, deviceId), digitalPin(digitalPin), analogPin(analogPin){

}

bool IRReflectorModule::getDigitalValue(){
    return digitalValue;
}

int IRReflectorModule::getAnalogValue(){
    return analogValue;
}

std::string IRReflectorModule::getDeviceName(){
    if(analogPin.length() == 0){
        return "IRReflectorModule(" + digitalPin + ")";
    }else{
        return "IRReflectorModule(" + digitalPin + ", " + analogPin + ")";
    }
}

void IRReflectorModule::applyDefaultState(){
    digitalValue = false;
    analogValue = 0;
}

std::vector<uint8_t> IRReflectorModule::getCreateData(){
    bool isDigPinAnalog = digitalPin[0] == 'A' || digitalPin[0] == 'a';
    int dp = 0, ap = 0;
    try{
        if(isDigPinAnalog){
            dp = std::stoi(digitalPin.substr(1, digitalPin.length() - 1));
        }else{  
            dp = std::stoi(digitalPin);
        }
    }catch(const std::runtime_error &e){
        Logger::logWarningFrom(getDeviceName(), "Invalid digital pin number");
        return {};
    }
    if(analogPin.length() == 0){
        ap = 255; // 255 means analog disabled
    }else{
        try{
            // Allow analog pin to either start with 'A' or not
            if(analogPin[0] == 'A' || analogPin[0] == 'a'){
                ap = std::stoi(analogPin.substr(1, analogPin.length() - 1));
            }else{
                ap = std::stoi(analogPin);
            }
        }catch(const std::runtime_error &e){
            Logger::logWarningFrom(getDeviceName(), "Invalid analog pin number");
        }
    }

    // ADDIRREFLECTOR,is_dig_analog,digital_pin,analog_pin
    std::vector<uint8_t> data = stringToData("ADDIRREFLECTOR");
    data.push_back(isDigPinAnalog);
    data.push_back(dp);
    data.push_back(ap);
    return data;
}

void IRReflectorModule::handleData(const std::vector<uint8_t> &data){
    // data = deviceId, data..., crc, crc
    if(data.size() >= 6){
        // Has digital and analog values
        digitalValue = data[1];
        analogValue = Conversions::convertDataToInt16(data, 2, true);
    }else if(data.size() >= 4){
        // Has only digital value
        digitalValue = data[1];
    }
}
