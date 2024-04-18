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

#include <arpirobot/arduino/iface/ArduinoUartInterface.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/robot/BaseRobot.hpp>
#include <arpirobot/core/io/Io.hpp>
#include <functional>
#include <algorithm>
#include <cstring>

using namespace arpirobot;

ArduinoUartInterface::ArduinoUartInterface(std::string port, int baud) : 
        IoDevice(), port(port), baud(baud){
    // Need a mutable C string
    portCStr = new char[port.length() + 1];
    strcpy(portCStr, port.c_str());
}

ArduinoUartInterface::~ArduinoUartInterface(){
    stopFromChildDestructor();
    delete portCStr;
}

void ArduinoUartInterface::open() {
    if(handle < 0){
        handle = Io::uartOpen(portCStr, baud);
    }
}

void ArduinoUartInterface::close() {
    if(isOpen())
        Io::uartClose(handle);
}

bool ArduinoUartInterface::isOpen() {
	return handle >= 0;
}

int ArduinoUartInterface::available() {
    if(handle < 0){
        auto msg = "Invalid UART handle in ArduinoUART interface.";
        Logger::logErrorFrom(getDeviceName(), msg);
        throw std::runtime_error(msg);
    }
    return Io::uartAvailable(handle);
}

uint8_t ArduinoUartInterface::readOne() {
    if(handle < 0){
        auto msg = "Invalid UART handle in ArduinoUART interface.";
        Logger::logErrorFrom(getDeviceName(), msg);
        throw std::runtime_error(msg);
    }
    return Io::uartReadByte(handle);
}

std::vector<uint8_t> ArduinoUartInterface::readAll() {
    if(handle < 0){
        auto msg = "Invalid UART handle in ArduinoUART interface.";
        Logger::logErrorFrom(getDeviceName(), msg);
        throw std::runtime_error(msg);
    }
    int a = available();
	std::vector<uint8_t> data;
    data.reserve(a);
    for(int i = 0; i < a; ++i){
        data.push_back(readOne());
    }
    return data;
}

void ArduinoUartInterface::write(const uint8_t &b) {
    if(handle < 0){
        auto msg = "Invalid UART handle in ArduinoUART interface.";
        Logger::logErrorFrom(getDeviceName(), msg);
        throw std::runtime_error(msg);
    }

    // Some boards, cables, power supplies, etc may cause scenarios where writes fail.
    // Allow three retries
    for(uint8_t i = 0; i < 3; ++i){
        try{
            Io::uartWriteByte(handle, b);
            break;
        }catch(const WriteFailedException &e){
            if(i == 2)
                throw e;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }
}

std::string ArduinoUartInterface::getDeviceName() {
	return "ArduinoUartInterface(" + port + ", " + std::to_string(baud) + ")";
}
