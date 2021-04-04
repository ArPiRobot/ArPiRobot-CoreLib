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

#include <arpirobot/arduino/sensor/Mpu6050Imu.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/conversions.hpp>

using namespace arpirobot;


Mpu6050Imu::Mpu6050Imu(bool createDevice, int deviceId) : ArduinoDevice(createDevice, deviceId){

}

void Mpu6050Imu::calibrate(uint16_t samples){
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

double Mpu6050Imu::getGyroX(){
    return gyroX + gyroXOffset;
}

double Mpu6050Imu::getGyroY(){
    return gyroY + gyroYOffset;
}

double Mpu6050Imu::getGyroZ(){
    return gyroZ + gyroZOffset;
}

double Mpu6050Imu::getAccelX(){
    return accelX;
}

double Mpu6050Imu::getAccelY(){
    return accelY;
}

double Mpu6050Imu::getAccelZ(){
    return accelZ;
}

void Mpu6050Imu::setGyroX(double newGyroX){
    gyroXOffset = newGyroX - gyroX;
}

void Mpu6050Imu::setGyroY(double newGyroY){
    gyroYOffset = newGyroY - gyroY;
}

void Mpu6050Imu::setGyroZ(double newGyroZ){
    gyroZOffset = newGyroZ - gyroZ;
}

std::string Mpu6050Imu::getDeviceName(){
    return "Mpu6050Imu";
}

void Mpu6050Imu::applyDefaultState(){
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

std::vector<uint8_t> Mpu6050Imu::getCreateData(){
    return stringToData("ADDMPU6050");
}

void Mpu6050Imu::handleData(const std::vector<uint8_t> &data){
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
