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

#include <arpirobot/arduino/sensor/VoltageMonitor.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/conversions.hpp>

using namespace arpirobot;


VoltageMonitor::VoltageMonitor(std::string pin, double vboard, int r1, int r2, bool createDevice, int deviceId) : 
        ArduinoDevice(createDevice, deviceId), pin(pin), vboard(vboard), r1(r1), r2(r2){
    deviceName = "VoltageMonitor(" + this->pin + ")";
}

VoltageMonitor::VoltageMonitor(int pin, double vboard, int r1, int r2, bool createDevice, int deviceId) : 
        ArduinoDevice(createDevice, deviceId), pin(std::to_string(pin)), vboard(vboard), r1(r1), r2(r2){
    deviceName = "VoltageMonitor(" + this->pin + ")";
}

double VoltageMonitor::getVoltage(){
    return voltage;
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
        if(isMainVmon()){
            sendMainBatteryVoltage(voltage);
        }
    }
}
