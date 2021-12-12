/*
 * Copyright 2021 Marcus Behel
 *
 * This file is part of ArPiRobot-CoreLib.
 * 
 * ArPiRobot-CoreLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ArPiRobot-CoreLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with ArPiRobot-CoreLib.  If not, see <https://www.gnu.org/licenses/>. 
 */

#include <arpirobot/arduino/device/ArduinoDevice.hpp>
#include <arpirobot/core/log/Logger.hpp>

using namespace arpirobot;


ArduinoDevice::ArduinoDevice(bool createDevice, int deviceId) : createDevice(createDevice), deviceId(deviceId){

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
    try{
        // Add '-' to the front to indicate that this data is to be sent to a device
        // Then add the id of the device
        std::vector<uint8_t> fullData;
        fullData.reserve(data.size() + 2);
        fullData.push_back('-');
        fullData.push_back((uint8_t)deviceId);
        for(size_t i = 0; i < data.size(); ++i){
            fullData.push_back(data[i]);
        }
        return true;
    }catch(std::exception &e){
        Logger::logDebugFrom(getDeviceName(), e.what());
        return false;
    }
}

bool ArduinoDevice::sendData(const std::string &data){
    std::vector<uint8_t> realData;
    realData.reserve(data.length());
    for(size_t i = 0; i < data.length(); ++i){
        realData.push_back(data[i]);
    }
    return sendData(realData);
}
