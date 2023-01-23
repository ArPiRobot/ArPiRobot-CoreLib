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

#include <arpirobot/devices/ina260/INA260PowerSensor.hpp>

#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/robot/BaseRobot.hpp>
#include <arpirobot/core/io/Io.hpp>

#include <stdexcept>
#include <thread>
#include <chrono>
#include <functional>

using namespace arpirobot;
using namespace arpirobot::internal;


////////////////////////////////////////////////////////////////////////////////
/// Internal use AdafruitINA260
////////////////////////////////////////////////////////////////////////////////

// Registers
const int AdafruitINA260::REG_CONFIG = 0x00;
const int AdafruitINA260::REG_CURRENT = 0x01;
const int AdafruitINA260::REG_BUSVOLTAGE = 0x02;
const int AdafruitINA260::REG_POWER = 0x03;
const int AdafruitINA260::REG_MASK_ENABLE = 0x06;
const int AdafruitINA260::REG_ALERT_LIMIT = 0x07;
const int AdafruitINA260::REG_MFG_UID = 0xFE;
const int AdafruitINA260::REG_DIE_UID = 0xFF;

// INA260 Mode values
const int AdafruitINA260::MODE_SHUTDOWN = 0x00;
const int AdafruitINA260::MODE_TRIGGERED = 0x03;
const int AdafruitINA260::MODE_CONTINUOUS = 0x07;

// INA260 Conversion Time values
const int AdafruitINA260::TIME_140_US = 0;
const int AdafruitINA260::TIME_204_US = 1;
const int AdafruitINA260::TIME_332_US = 2;
const int AdafruitINA260::TIME_558_US = 3;
const int AdafruitINA260::TIME_1_1_MS = 4;
const int AdafruitINA260::TIME_2_116_MS = 5;
const int AdafruitINA260::TIME_4_156_MS = 6;
const int AdafruitINA260::TIME_8_244_MS = 7;

// INA260 Averaging Count values
const int AdafruitINA260::COUNT_1 = 0;
const int AdafruitINA260::COUNT_2 = 1;
const int AdafruitINA260::COUNT_16 = 2;
const int AdafruitINA260::COUNT_64 = 3;
const int AdafruitINA260::COUNT_128 = 4;
const int AdafruitINA260::COUNT_256 = 5;
const int AdafruitINA260::COUNT_512 = 6;
const int AdafruitINA260::COUNT_1024 = 7;

AdafruitINA260::AdafruitINA260(int address, int bus) : IoDevice(){
    handle = Io::i2cOpen(bus, address);

    // Make sure there is a device at that address (will throw exception if read fails)
    Io::i2cReadByte(handle);
}

AdafruitINA260::AdafruitINA260::~AdafruitINA260(){
    close();
}

bool AdafruitINA260::begin(){
    int manufacturerId = i2cReadWordHelper(REG_MFG_UID);

    // Device ID is the upper 12 bits of this register
    int deviceId = i2cReadWordHelper(REG_DIE_UID) & 0xFFF0;
    deviceId >>= 4;

    // Verify the device
    if((manufacturerId != 0x5449) || (deviceId != 0x227)){
        Logger::logInfo("MFG: " + std::to_string(manufacturerId));
        Logger::logInfo("DEV: " + std::to_string(deviceId));
        return false;
    }

    reset();
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    return true;
}

void AdafruitINA260::reset(){
    int value = i2cReadWordHelper(REG_CONFIG);
    value |= 0x8000;
    i2cWriteWordHelper(REG_CONFIG, value);
}

void AdafruitINA260::close(){
    try{
        Io::i2cClose(handle);
    }catch(const std::exception &e){
        // Silently fail
    }
}

double AdafruitINA260::readCurrent(){
    return i2cReadWordHelper(REG_CURRENT) * 1.25;
}

double AdafruitINA260::readPower(){
    return i2cReadWordHelper(REG_POWER) * 10;
}

double AdafruitINA260::readBusVoltage(){
    return i2cReadWordHelper(REG_BUSVOLTAGE) * 1.25;
}

void AdafruitINA260::setMode(int mode){
    int value = i2cReadWordHelper(REG_CONFIG);

    // Mask out lowest 3 bits
    value &= (~0x07);

    // Set new value for lowest 3 bits
    value |= (mode & 0x7);

    i2cWriteWordHelper(REG_CONFIG, value);
}

int AdafruitINA260::getMode(){
    return i2cReadWordHelper(REG_CONFIG) & 0x7;
}

void AdafruitINA260::setAveragingCount(int count){
    // AvgCount is bits 9-11 in config register

    int value = i2cReadWordHelper(REG_CONFIG);

    // Shift the data to align with the bits that will be set
    count <<= 9;
    count &= 0xE00; // Mask out all other bits in the data

    // Mask out bits that will be replaced
    value &= (~0xE00);

    // Set the new value for the bits
    value |= count;

    // Write
    i2cWriteWordHelper(REG_CONFIG, value);
}

int AdafruitINA260::getAveragingCount(){
    return i2cReadWordHelper(REG_CONFIG) & 0xE00;
}

void AdafruitINA260::setCurrentConversionTime(int time){
    // Current conversion time is bits 3-5
    int value = i2cReadWordHelper(REG_CONFIG);

    // Align data
    time <<= 3;
    time &= 0x38;

    // Mask out and replace data
    value &= (~0x38);
    value |= time;

    // Write
    i2cWriteWordHelper(REG_CONFIG, (short)value);
}

int AdafruitINA260::getCurrentConversionTime(){
    return i2cReadWordHelper(REG_CONFIG) & 0x38;
}

void AdafruitINA260::setVoltageConversionTime(int time){
    // Voltage conversion time is bits 6-8
    int value = i2cReadWordHelper(REG_CONFIG);

    // Align data
    time <<= 6;
    time &= 0x1C0;

    // Mask out and replace data
    value &= (~0x1C0);
    value |= time;

    // Write
    i2cWriteWordHelper(REG_CONFIG, (short)value);
}

int AdafruitINA260::getVoltageConversiontime(){
    return i2cReadWordHelper(REG_CONFIG) & 0x1C0;
}

bool AdafruitINA260::conversionReady(){
    return i2cReadWordHelper(REG_MASK_ENABLE) & 0x4;
}

int AdafruitINA260::i2cReadWordHelper(int reg){
    // pigpio reads little endian. Need to read big endian.
    int value = Io::i2cReadReg16(handle, reg);
    return (value >> 8 & 0x00FF) | (value << 8 & 0xFF00);
}

void AdafruitINA260::i2cWriteWordHelper(int reg, int data){
    data = (data >> 8 & 0x00FF) | (data << 8 & 0xFF00);
    Io::i2cWriteReg16(handle, reg, data);
}


////////////////////////////////////////////////////////////////////////////////
/// INA260PowerSensor
////////////////////////////////////////////////////////////////////////////////

INA260PowerSensor::INA260PowerSensor(int bus) : bus(bus){
    deviceName = "INA260PowerSensor";
    BaseRobot::beginWhenReady(this);
}

INA260PowerSensor::~INA260PowerSensor(){
    stop = true;
}

double INA260PowerSensor::getCurrent(){
    return current;
}

double INA260PowerSensor::getVolgate(){
    return voltage;
}

double INA260PowerSensor::getPower(){
    return power;
}

void INA260PowerSensor::begin(){
    try{
        if(bus == -1)
            bus = Io::getDefaultI2cBus();
        sensor = std::make_shared<AdafruitINA260>(0x40, bus);
    }catch(const std::exception &e){
        Logger::logErrorFrom(getDeviceName(), "Failed to initialize sensor.");
        Logger::logDebugFrom(getDeviceName(), e.what());
    }
    if(!sensor->begin()){
        Logger::logErrorFrom(getDeviceName(), "Failed to initialize sensor.");
        Logger::logDebugFrom(getDeviceName(), "Incorrect device at given adddress.");
        return;
    }
    BaseRobot::runOnceSoon(std::bind(&INA260PowerSensor::feed, this));
}

bool INA260PowerSensor::isEnabled(){
    return true;
}

bool INA260PowerSensor::shouldMatchRobotState(){
    return false;
}

bool INA260PowerSensor::shouldDisableWithWatchdog(){
    return false;
}

void INA260PowerSensor::enable(){

}

void INA260PowerSensor::disable(){

}

void INA260PowerSensor::feed(){
    while(!stop && BaseRobot::exists){
        try{
            current = sensor->readCurrent();
            voltage = sensor->readBusVoltage() / 1000.0;
            power = sensor->readPower();

            if(isMainVmon())
                sendMainBatteryVoltage(voltage);
        }catch(const std::exception &e){
            Logger::logWarningFrom(getDeviceName(), "Failed to read data.");
            Logger::logDebugFrom(getDeviceName(), e.what());
        }

        // Instead of running this every 50ms (using scheduler repeated task)
        // ensure 50ms between end of previous run and start of next
        // this helps to reduce CPU usage (especially on pi zero)
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
