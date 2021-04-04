#include <arpirobot/arduino/sensor/Ultrasonic4Pin.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/conversions.hpp>

using namespace arpirobot;

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
