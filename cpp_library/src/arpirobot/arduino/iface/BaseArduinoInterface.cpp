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

#include <arpirobot/arduino/iface/BaseArduinoInterface.hpp>
#include <arpirobot/arduino/device/ArduinoDevice.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/robot/BaseRobot.hpp>
#include <functional>
#include <algorithm>
#include <cstring>

using namespace arpirobot;


const uint8_t BaseArduinoInterface::START_BYTE = 253;
const uint8_t BaseArduinoInterface::END_BYTE = 254;
const uint8_t BaseArduinoInterface::ESCAPE_BYTE = 255;

const std::vector<uint8_t> BaseArduinoInterface::MSG_START = {'S', 'T', 'A', 'R', 'T'};
const std::vector<uint8_t> BaseArduinoInterface::MSG_ADD = {'A', 'D', 'D'};
const std::vector<uint8_t> BaseArduinoInterface::MSG_ADDSUCCESS = {'A', 'D', 'D', 'S', 'U', 'C', 'C', 'E', 'S', 'S'};

const std::vector<uint8_t> BaseArduinoInterface::CMD_RESET = {'R', 'E', 'S', 'E', 'T'};
const std::vector<uint8_t> BaseArduinoInterface::CMD_END = {'E', 'N', 'D'};

BaseArduinoInterface::~BaseArduinoInterface(){
    if(processThread != nullptr){
        processThread->join();
        delete processThread;
    }
}

bool BaseArduinoInterface::begin(){
    initialized = true;
    parseStarted = false;
    parseEscaped = false;

    try{
        if(!isOpen())
            open();
    }catch(const std::exception &e){
        Logger::logErrorFrom(getDeviceName(), "Unable to open arduino interface.");
        Logger::logDebugFrom(getDeviceName(), e.what());
        return false;
    }
    
    if(!isOpen()){
        Logger::logErrorFrom(getDeviceName(), "Unable to open arduino interface. Unknown error.");
        return false;
    }

    Logger::logDebugFrom(getDeviceName(), "Opened interface.");

    try{
        // Clear any start message from the buffer
        // This will be there from when the arduino reset on opening UART
        waitForMessage(MSG_START, 3000);

        // Begin by sending reset command (just in case it did not reset on opening UART)
        // Some devices don't reset on UART (Teensy for example)
        writeData(CMD_RESET);

        // Wait for START message
        auto msg = waitForMessage(MSG_START, 5000);
        if(msg.size() == 0){
            Logger::logWarningFrom(getDeviceName(), "Timed out while waiting for the Arduino to become ready.");
            return false;
        }
    }catch(const std::exception &e){
        Logger::logWarningFrom(getDeviceName(), "Error while configuring devices.");
        Logger::logDebugFrom(getDeviceName(), e.what());
        return false;
    }

    Logger::logDebugFrom(getDeviceName(), "Arduino is ready. Creating devices.");

    try{
        for(auto dev : devices){
            if(dev->createDevice){
                auto createData = dev->getCreateData();
                
                // Empty create data means the device failed to generate its create data
                // The device should handle error messages
                if(createData.size() > 0){
                    writeData(createData);
                    auto msg = waitForMessage(MSG_ADD, 5000);
                    if(msg.size() == 0){
                        Logger::logWarningFrom(getDeviceName(), "Timed out while adding device " + dev->getDeviceName());
                        dev->setDeviceId(-1);
                    }else if(msgStartsWith(msg, MSG_ADDSUCCESS)){
                        dev->setDeviceId(msg[10]);
                        dev->applyDefaultState();
                        Logger::logDebugFrom(dev->getDeviceName(), "Created device with ID " + std::to_string(dev->deviceId));
                    }else{
                        Logger::logWarningFrom(getDeviceName(), "Arduino failed to add device " + dev->getDeviceName());
                        dev->setDeviceId(-1);
                    }
                }
            }
        }
    }catch(const std::exception &e){
        Logger::logWarningFrom(getDeviceName(), "Error while configuring devices.");
        Logger::logDebugFrom(getDeviceName(), e.what());
        return false;
    }

    Logger::logDebugFrom(getDeviceName(), "Done creating devices. Starting sensor processing.");

    try{
        writeData(CMD_END);
        auto msg = waitForMessage(CMD_END, 5000);
        if(msg.size() == 0){
            Logger::logWarningFrom(getDeviceName(), "Timed out while starting sensor processing.");
            return false;
        }
    }catch(const std::exception &e){
        Logger::logWarningFrom(getDeviceName(), "Error while configuring devices.");
        Logger::logDebugFrom(getDeviceName(), e.what());
        return false;
    }

    Logger::logDebugFrom(getDeviceName(), "Sensor processing started successfully.");

    if(processThread != nullptr){
        arduinoReady = false;
        processThread->join();
        delete processThread;
    }
    arduinoReady = true;
    processThread = new std::thread(std::bind(&BaseArduinoInterface::run, this));
    return true;
}

void BaseArduinoInterface::addDevice(ArduinoDevice *device){
    if(initialized){
        Logger::logWarningFrom(getDeviceName(), "Attempted to add device after calling begin. This will not work.");
        return;
    }
    if(device->arduino != nullptr){
        Logger::logWarningFrom(getDeviceName(), 
            "Attempted to add a device, but it had already been added to an arduino. This is not allowed.");
        return;
    }
    if(std::find(devices.begin(), devices.end(), device) == devices.end()){
        device->setArduino(this);
        devices.push_back(device);
    }
}

bool BaseArduinoInterface::isReady(){
    return arduinoReady;
}

void BaseArduinoInterface::sendFromDevice(uint8_t deviceId, std::vector<uint8_t> data){
    try{
        std::vector<uint8_t> sendData;
        sendData.reserve(data.size() + 2);
        sendData.push_back('-');
        sendData.push_back(deviceId);
        sendData.insert(sendData.end(), data.begin(), data.end());
        writeData(sendData);
    }catch(std::exception &e){
        Logger::logWarningFrom(getDeviceName(), "Failed to send message to device with ID " + std::to_string(deviceId) + ".");
        Logger::logDebugFrom(getDeviceName(), e.what());
    }
}

void BaseArduinoInterface::run(){
    while(arduinoReady){
        try{
            if(readData() && checkData()){
                // Special case check
                if(msgEquals(readDataset, MSG_START)){
                    Logger::logWarningFrom(getDeviceName(), "Arduino was reset while running. Sensor data is now INVALID! Will reconfigure.");
                    break;
                }

                int deviceId = readDataset[0];
                for(auto dev : devices){
                    if(dev->deviceId == deviceId){
                        dev->handleData(readDataset);
                        break;
                    }
                }
            }
        }catch(const std::exception &e){
            Logger::logWarningFrom(getDeviceName(), "Lost communication with the arduino. Sensor data is now INVALID!. Will reconfigure.");
            Logger::logDebugFrom(getDeviceName(), e.what());
            arduinoReady = false;
        }
    }

    // The above loop exits when communication is lost
    // Have begin run asynchronously
    BaseRobot::runOnceSoon(std::bind(&BaseArduinoInterface::begin, this));
}

uint16_t BaseArduinoInterface::calcCCittFalse(const std::vector<uint8_t> &data, size_t len){
    uint16_t crc = 0xFFFF;
    int pos = 0;
    while(pos < len){
        uint8_t b = data[pos];
        for(int i = 0; i < 8; ++i){
            uint8_t bit = ((b >> (7 - i) & 1) == 1);
            uint8_t c15 = ((crc >> 15 & 1) == 1);
            crc <<= 1;
            if(c15 ^ bit){
                crc ^= 0x1021;
            }
        }
        pos++;
    }
    return crc;
}

void BaseArduinoInterface::writeData(const std::vector<uint8_t> &data){
    write(START_BYTE);
    for(int i = 0; i < data.size(); ++i){
        const uint8_t &b = data[i];
        if(b == START_BYTE || b == END_BYTE || b == ESCAPE_BYTE)
            write(ESCAPE_BYTE);
        write(b);
    }

    // Calculate CRC
    uint16_t crc = calcCCittFalse(data, data.size());
    const uint8_t crc_low = crc & 0x00FF;
    const uint8_t crc_high = crc >> 8 & 0x00FF;
    if(crc_high == START_BYTE || crc_high == END_BYTE || crc_high == ESCAPE_BYTE)
        write(ESCAPE_BYTE);
    write(crc_high);
    if(crc_low == START_BYTE || crc_low == END_BYTE || crc_low == ESCAPE_BYTE)
        write(ESCAPE_BYTE);
    write(crc_low);

    write(END_BYTE);
}

bool BaseArduinoInterface::readData(){
    uint8_t b = readOne();
    if(parseEscaped){
        // Only allow valid escape sequences
        if(b == START_BYTE || b == END_BYTE || b == ESCAPE_BYTE)
            workingBuffer.push_back(b);
        parseEscaped = false;
    }else{
        if(b == START_BYTE){
            if(parseStarted){
                // Got a second start byte. Trashing buffer.
                workingBuffer.clear();
            }
            parseStarted = true;
        }else if(b == END_BYTE && parseStarted){
            readDataset.clear();
            for(size_t i = 0; i < workingBuffer.size(); ++i){
                readDataset.push_back(workingBuffer[i]);
            }
            workingBuffer.clear();
            parseStarted = false;
            return true;
        }else if(b == ESCAPE_BYTE && parseStarted){
            parseEscaped = true;
        }else if(b != START_BYTE && b != END_BYTE && b != ESCAPE_BYTE){
            workingBuffer.push_back(b);
        }
    }
    return false;
}

bool BaseArduinoInterface::checkData(){

    if(readDataset.size() == 0)
        return false;
    // CRC is sent big endian (high_byte, low_byte)
    uint16_t readCrc = readDataset[readDataset.size() - 2] << 8 | readDataset[readDataset.size() - 1];
    uint16_t calcCrc = calcCCittFalse(readDataset, readDataset.size() - 2);

    return readCrc == calcCrc;
}

std::vector<uint8_t> BaseArduinoInterface::waitForMessage(const std::vector<uint8_t> &prefix, int timeoutMs){
    auto start = std::chrono::steady_clock::now();
    while(true){
        while(available()  == 0){
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - start).count();
            if(elapsed >= timeoutMs){
                return {};
            }
        }
        if(readData() && checkData()){
            if(msgStartsWith(readDataset, prefix)){
                return readDataset;
            }
        }
    }
}

bool BaseArduinoInterface::msgStartsWith(const std::vector<uint8_t> &msg, const std::vector<uint8_t> &prefix){
    // If prefix is longer than msg then msg cannot start with prefix
    if(prefix.size() > msg.size())
        return false;
    for(size_t i = 0; i < prefix.size(); ++i){
        if(prefix[i] != msg[i])
            return false;
    }
    return true;
}

bool BaseArduinoInterface::msgEquals(const std::vector<uint8_t> &msg1, const std::vector<uint8_t> &msg2){
    if(msg1.size() != msg2.size())
        return false;
    for(size_t i = 0; i < msg1.size(); ++i){
        if(msg1[i] != msg2[i])
            return false;
    }
    return true;
}
