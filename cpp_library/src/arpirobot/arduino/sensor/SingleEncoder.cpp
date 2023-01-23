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

#include <arpirobot/arduino/sensor/SingleEncoder.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/conversions.hpp>

using namespace arpirobot;


SingleEncoder::SingleEncoder(int pin, bool useInternalPullup, bool createDevice, int deviceId) : 
        ArduinoDevice(createDevice, deviceId), pin(std::to_string(pin)), useInternalPullup(useInternalPullup){
    deviceName = "SingleEncoder(" + this->pin + ")";
}

SingleEncoder::SingleEncoder(std::string pin, bool useInternalPullup, bool createDevice, int deviceId) : 
        ArduinoDevice(createDevice, deviceId), pin(pin), useInternalPullup(useInternalPullup){
    deviceName = "SingleEncoder(" + this->pin + ")";
}

int32_t SingleEncoder::getPosition(){
    return count + countOffset;
}

void SingleEncoder::setPosition(int32_t newPosition){
    countOffset = newPosition - count;
}

float SingleEncoder::getVelocity(){
    return velocity;
}

void SingleEncoder::applyDefaultState(){
    count = 0;
    countOffset = 0;
    velocity = 0.0f;
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
    if(data.size() >= 11){
        // New format. Contains 32-bit count (int) and 32-bit velocity (float)
        count = Conversions::convertDataToInt32(data, 1, true);
        velocity = Conversions::convertDataToFloat(data, 5, true);
    }else if(data.size() >= 5){
        // Old format. Count is 16-bit. No velocity data
        // At least 2 bytes of data. Position is included
        count = Conversions::convertDataToInt16(data, 1, true);
    }
}
