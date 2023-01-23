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

#include <arpirobot/arduino/sensor/QuadEncoder.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/conversions.hpp>

using namespace arpirobot;


QuadEncoder::QuadEncoder(int pinA, int pinB, bool useInternalPullup, bool createDevice, int deviceId) : 
        ArduinoDevice(createDevice, deviceId), pinA(std::to_string(pinA)), pinB(std::to_string(pinB)), 
        useInternalPullup(useInternalPullup){
    deviceName = "QuadEncoder(" + this->pinA + ", " + this->pinB + ")";
}

QuadEncoder::QuadEncoder(std::string pinA, int pinB, bool useInternalPullup, bool createDevice, int deviceId) : 
        ArduinoDevice(createDevice, deviceId), pinA(pinA), pinB(std::to_string(pinB)), 
        useInternalPullup(useInternalPullup){
    deviceName = "QuadEncoder(" + this->pinA + ", " + this->pinB + ")";
}

QuadEncoder::QuadEncoder(int pinA, std::string pinB, bool useInternalPullup, bool createDevice, int deviceId) : 
        ArduinoDevice(createDevice, deviceId), pinA(std::to_string(pinA)), pinB(pinB), 
        useInternalPullup(useInternalPullup){
    deviceName = "QuadEncoder(" + this->pinA + ", " + this->pinB + ")";
}

QuadEncoder::QuadEncoder(std::string pinA, std::string pinB, bool useInternalPullup, bool createDevice, int deviceId) : 
        ArduinoDevice(createDevice, deviceId), pinA(pinA), pinB(pinB), useInternalPullup(useInternalPullup){
    deviceName = "QuadEncoder(" + this->pinA + ", " + this->pinB + ")";
}

int32_t QuadEncoder::getPosition(){
    return count + countOffset;
}

void QuadEncoder::setPosition(int32_t newPosition){
    countOffset = newPosition - count;
}

float QuadEncoder::getVelocity(){
    return velocity;
}

void QuadEncoder::applyDefaultState(){
    count = 0;
    countOffset = 0;
    velocity = 0.0f;
}

std::vector<uint8_t> QuadEncoder::getCreateData(){
    uint8_t isAnalogA = pinA[0] == 'A' || pinA[0] == 'a';
    uint8_t isAnalogB = pinB[0] == 'A' || pinB[0] == 'a';
    uint8_t pA, pB;
    try{
        if(isAnalogA){
            pA = std::stoi(pinA.substr(1, pinA.length() - 1));
        }else{
            pA = std::stoi(pinA);
        }
        if(isAnalogB){
            pB = std::stoi(pinB.substr(1, pinB.length() - 1));
        }else{
            pB = std::stoi(pinB);
        }
    }catch(const std::runtime_error &e){
        Logger::logWarningFrom(getDeviceName(), "Invalid pin number specified.");
        return {};
    }

    // ADDSENC,is_analog,pin,pullup
    std::vector<uint8_t> data = stringToData("ADDQENC");
    data.push_back(isAnalogA);
    data.push_back(pA);
    data.push_back(isAnalogB);
    data.push_back(pB);
    data.push_back(useInternalPullup);
    return data;
}

void QuadEncoder::handleData(const std::vector<uint8_t> &data){
    // Buffer contains deviceId, data..., crc, crc
    if(data.size() >= 11){
        // Contains 32-bit count (int) and 32-bit velocity (float)
        count = Conversions::convertDataToInt32(data, 1, true);
        velocity = Conversions::convertDataToFloat(data, 5, true);
    }
}
