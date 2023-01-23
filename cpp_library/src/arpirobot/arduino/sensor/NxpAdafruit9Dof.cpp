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

#include <arpirobot/arduino/sensor/NxpAdafruit9Dof.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/conversions.hpp>

using namespace arpirobot;


NxpAdafruit9Dof::NxpAdafruit9Dof(bool createDevice, int deviceId) : ArduinoDevice(createDevice, deviceId){
    deviceName = "NxpAdafruit9Dof";
}

void NxpAdafruit9Dof::calibrate(uint16_t samples){
    if(arduino == nullptr)
        return;
    Logger::logInfoFrom(getDeviceName(), "Starting calibration. Will not get data until calibration is complete.");
    std::vector<uint8_t> msg;
    msg.reserve(3);
    msg.push_back('C'); // Calibrate command
    auto s = Conversions::convertInt16ToData(samples, true);
    msg.insert(msg.end(), s.begin(), s.end());
    // msg = 'C', samples_little_endian
    arduino->sendFromDevice(this->deviceId, msg);
}

double NxpAdafruit9Dof::getGyroX(){
    return gyroX + gyroXOffset;
}

double NxpAdafruit9Dof::getGyroY(){
    return gyroY + gyroYOffset;
}

double NxpAdafruit9Dof::getGyroZ(){
    return gyroZ + gyroZOffset;
}

double NxpAdafruit9Dof::getAccelX(){
    return accelX;
}

double NxpAdafruit9Dof::getAccelY(){
    return accelY;
}

double NxpAdafruit9Dof::getAccelZ(){
    return accelZ;
}

void NxpAdafruit9Dof::setGyroX(double newGyroX){
    gyroXOffset = newGyroX - gyroX;
}

void NxpAdafruit9Dof::setGyroY(double newGyroY){
    gyroYOffset = newGyroY - gyroY;
}

void NxpAdafruit9Dof::setGyroZ(double newGyroZ){
    gyroZOffset = newGyroZ - gyroZ;
}

void NxpAdafruit9Dof::applyDefaultState(){
    gyroX = 0;
    gyroY = 0;
    gyroZ = 0;
    accelX = 0;
    accelY = 0;
    accelZ = 0;

    gyroXOffset = 0;
    gyroYOffset = 0;
    gyroZOffset = 0;
}

std::vector<uint8_t> NxpAdafruit9Dof::getCreateData(){
    return stringToData("ADDNXPADA9DOF");
}

void NxpAdafruit9Dof::handleData(const std::vector<uint8_t> &data){
    // If at least 24 bytes of data (deviceId, data..., crc, crc)
    if(data.size() >= 27){
        gyroX = Conversions::convertDataToFloat(data, 1, true);
        gyroY = Conversions::convertDataToFloat(data, 5, true);
        gyroZ = Conversions::convertDataToFloat(data, 9, true);

        accelX = Conversions::convertDataToFloat(data, 13, true);
        accelY = Conversions::convertDataToFloat(data, 17, true);
        accelZ = Conversions::convertDataToFloat(data, 21, true);
    }
}

