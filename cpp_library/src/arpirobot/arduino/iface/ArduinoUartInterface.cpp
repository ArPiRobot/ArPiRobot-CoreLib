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
#include <functional>
#include <algorithm>
#include <pigpio.h>
#include <cstring>

using namespace arpirobot;

ArduinoUartInterface::ArduinoUartInterface(std::string port, int baud) : port(port), baud(baud){

}

void ArduinoUartInterface::open() {
    if(handle < 0){
        char tty[port.length() + 1];
        strcpy(tty, port.c_str());
        handle = serOpen(tty, baud, 0);
    }
}

void ArduinoUartInterface::close() {
	if(handle >= 0){
        serClose(handle);
    }
}

bool ArduinoUartInterface::isOpen() {
	return handle >= 0;
}

int ArduinoUartInterface::available() {
    if(handle < 0)
        return 0;
    return serDataAvailable(handle);
}

uint8_t ArduinoUartInterface::readOne() {
    // Note: Would be MUCH better to do this with blocking io...
    while(!available()){
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    int b = serReadByte(handle);
    if(b < 0){
        throw std::runtime_error("Failed to read from serial port.");
    }
    return (uint8_t) b;
}

std::vector<uint8_t> ArduinoUartInterface::readAll() {
    int a = available();
	std::vector<uint8_t> data;
    data.reserve(a);
    for(int i = 0; i < a; ++i){
        data.push_back(readOne());
    }
    return data;
}

void ArduinoUartInterface::write(const uint8_t &b) {
	int res = serWriteByte(handle, b);
    if(res < 0){
        throw std::runtime_error("Failed to write to serial port.");
    }
}

std::string ArduinoUartInterface::getDeviceName() {
	return "ArduinoUartInterface(" + port + ", " + std::to_string(baud) + ")";
}
