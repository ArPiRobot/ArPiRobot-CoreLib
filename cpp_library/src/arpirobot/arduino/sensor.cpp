#include <arpirobot/arduino/sensor.hpp>
#include <arpirobot/core/log.hpp>
#include <arpirobot/core/conversions.hpp>
#include <arpirobot/core/network.hpp>

using namespace arpirobot;


////////////////////////////////////////////////////////////////////////////////
/// VoltageMonitor
////////////////////////////////////////////////////////////////////////////////

VoltageMonitor::VoltageMonitor(std::string pin, double vboard, int r1, int r2, bool createDevice, int deviceId) : 
        ArduinoDevice(createDevice, deviceId), pin(pin), vboard(vboard), r1(r1), r2(r2){

}

VoltageMonitor::VoltageMonitor(int pin, double vboard, int r1, int r2, bool createDevice, int deviceId) : 
        ArduinoDevice(createDevice, deviceId), pin(std::to_string(pin)), vboard(vboard), r1(r1), r2(r2){

}

double VoltageMonitor::getVoltage(){
    return voltage;
}

void VoltageMonitor::makeMainVmon(){
    NetworkManager::setMainVmon((void*)this);
}

std::string VoltageMonitor::getDeviceName(){
    return "VoltageMonitor(" + pin + ")";
}

void VoltageMonitor::applyDefaultState(){
    voltage = 0;
}

std::vector<uint8_t> VoltageMonitor::getCreateData(){
    std::string pinStr;
    if(pin[0] == 'A' || pin[0] == 'a'){
        // Analog pin
        // Skip the 'A' when converting to int
        pinStr = pin.substr(1, pin.length() - 1);
    }
    try{
        uint8_t p = std::stoi(pinStr);

        std::vector<uint8_t> data = stringToData("ADDVMON");
        data.push_back(p);

        std::vector<uint8_t> vboardData = Conversions::convertFloatToData(vboard, false);
        for(uint8_t b : vboardData){
            data.push_back(b);
        }

        std::vector<uint8_t> r1Data = Conversions::convertInt32ToData(r1, false);
        for(uint8_t b : r1Data){
            data.push_back(b);
        }

        std::vector<uint8_t> r2Data = Conversions::convertInt32ToData(r2, false);
        for(uint8_t b : r2Data){
            data.push_back(b);
        }

        return data;
    }catch(const std::runtime_error &e){
        Logger::logWarningFrom(getDeviceName(), "Invalid pin number specified.");
        return {};
    }
    return {};
}

void VoltageMonitor::handleData(const std::vector<uint8_t> &data){
    if(data.size() >= 5){
        voltage = Conversions::convertDataToFloat(data, 1, true);
        if(NetworkManager::isMainVmon(this)){
            NetworkManager::sendMainBatteryVoltage(voltage);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
/// Ultrasonic4Pin
////////////////////////////////////////////////////////////////////////////////

Ultrasonic4Pin::Ultrasonic4Pin(int triggerPin, int echoPin, bool createDevice, int deviceId) : 
        ArduinoDevice(createDevice, deviceId), triggerPin(std::to_string(triggerPin)), echoPin(std::to_string(echoPin)){

}

Ultrasonic4Pin::Ultrasonic4Pin(int triggerPin, std::string echoPin, bool createDevice, int deviceId) : 
        ArduinoDevice(createDevice, deviceId), triggerPin(std::to_string(triggerPin)), echoPin(echoPin){

}

Ultrasonic4Pin::Ultrasonic4Pin(std::string triggerPin, int echoPin, bool createDevice, int deviceId) : 
        ArduinoDevice(createDevice, deviceId), triggerPin(triggerPin), echoPin(std::to_string(echoPin)){

}

Ultrasonic4Pin::Ultrasonic4Pin(std::string triggerPin, std::string echoPin, bool createDevice, int deviceId) : 
        ArduinoDevice(createDevice, deviceId), triggerPin(triggerPin), echoPin(echoPin){

}

int Ultrasonic4Pin::getDistance(){
    return distance;
}

std::string Ultrasonic4Pin::getDeviceName(){
    return "Ultrasonic4Pin(" + triggerPin + ", " + echoPin + ")";
}

void Ultrasonic4Pin::applyDefaultState(){
    distance = 0;
}

std::vector<uint8_t> Ultrasonic4Pin::getCreateData(){
    uint8_t triggerAnalog = echoPin[0] == 'A' || echoPin[0] == 'a';
    uint8_t echoAnalog = triggerPin[0] == 'A' || echoPin[0] == 'a';
    uint8_t tp, ep;

    try{
        if(triggerPin[0] == 'A' || triggerPin[0] == 'a'){
            tp = std::stoi(triggerPin.substr(1, triggerPin.length() - 1));
        }else{
            tp = std::stoi(triggerPin);
        }
    }catch(const std::runtime_error &e){
        Logger::logWarningFrom(getDeviceName(), "Invalid trigger pin number specified.");
        return {};
    }
    try{
        if(echoPin[0] == 'A' || triggerPin[0] == 'a'){
            ep = std::stoi(echoPin.substr(1, echoPin.length() - 1));
        }else{
            ep = std::stoi(echoPin);
        }
    }catch(const std::runtime_error &e){
        Logger::logWarningFrom(getDeviceName(), "Invalid echo pin number specified.");
    }

    // ADDUSONIC4,trigger_analog,trigger_pin,echo_analog,echo_pin
    std::vector<uint8_t> data = stringToData("ADDUSONIC4");
    data.push_back(triggerAnalog);
    data.push_back(tp);
    data.push_back(echoAnalog);
    data.push_back(ep);
    return data;
}

void Ultrasonic4Pin::handleData(const std::vector<uint8_t> &data){
    // Buffer will contain deviceId, data..., crc, crc
    if(data.size() >= 5){
        // At least 2 bytes of data
        // distance is 16-bit int, little endian
        distance = Conversions::convertDataToInt16(data, 1, true);
    }
}


////////////////////////////////////////////////////////////////////////////////
/// SingleEncoder
////////////////////////////////////////////////////////////////////////////////

SingleEncoder::SingleEncoder(int pin, bool useInternalPullup, bool createDevice, int deviceId) : 
        ArduinoDevice(createDevice, deviceId), pin(std::to_string(pin)), useInternalPullup(useInternalPullup){

}

SingleEncoder::SingleEncoder(std::string pin, bool useInternalPullup, bool createDevice, int deviceId) : 
        ArduinoDevice(createDevice, deviceId), pin(pin), useInternalPullup(useInternalPullup){

}

int SingleEncoder::getPosition(){
    return count + countOffset;
}

void SingleEncoder::setPosition(int currentPosition){
    if(count == currentPosition)
        return;
    countOffset = currentPosition - count;
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


////////////////////////////////////////////////////////////////////////////////
/// IRReflectorModule
////////////////////////////////////////////////////////////////////////////////

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
