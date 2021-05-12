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


#include <arpirobot/core/io/DummyIoProvider.hpp>
#include <arpirobot/core/log/Logger.hpp>

using namespace arpirobot;


DummyIoProvider::DummyIoProvider() : IoProvider(){
    Logger::logWarning("The dummy IO provider has been started. This provider CANNOT be used to" 
            "interact with devices. It is intended for debugging purposes only. It will only log IO "
            "operations. They will NOT actually be performed.");
    Logger::logDebugFrom("DummyIoProvider", "IO Provider initialized.");
}
        
DummyIoProvider::~DummyIoProvider(){
    Logger::logDebugFrom("DummyIoProvider", "IO Provider terminated.");
}

////////////////////////////////////////////////////////////////////////
/// GPIO & PWM
////////////////////////////////////////////////////////////////////////
void DummyIoProvider::gpioMode(unsigned int pin, unsigned int mode){
    Logger::logDebugFrom("DummyIoProvider", "gpioMode(" + std::to_string(pin) + ", " + std::to_string(mode) + ")");
}

void DummyIoProvider::gpioWrite(unsigned int pin, unsigned int state){
    Logger::logDebugFrom("DummyIoProvider", "gpioWrite(" + std::to_string(pin) + ", " + std::to_string(state) + ")");
}

unsigned int DummyIoProvider::gpioRead(unsigned int pin){
    Logger::logDebugFrom("DummyIoProvider", "gpioRead(" + std::to_string(pin) + ")");
    return 0;
}

void DummyIoProvider::gpioSetPwmFrequency(unsigned int pin, unsigned int frequency){
    Logger::logDebugFrom("DummyIoProvider", "gpioSetPwmFrequency(" + std::to_string(pin) + ", " + std::to_string(frequency) + ")");
}

void DummyIoProvider::gpioPwm(unsigned int pin, unsigned int value){
    Logger::logDebugFrom("DummyIoProvider", "gpioPwm(" + std::to_string(pin) + ", " + std::to_string(value) + ")");
}


////////////////////////////////////////////////////////////////////////
/// I2C
////////////////////////////////////////////////////////////////////////
unsigned int DummyIoProvider::i2cOpen(unsigned int bus, unsigned int address){
    Logger::logDebugFrom("DummyIoProvider", "i2cOpen(" + std::to_string(bus) + ", " + std::to_string(address) + ")");
    return 0;
}

void DummyIoProvider::i2cClose(unsigned int handle){
    Logger::logDebugFrom("DummyIoProvider", "i2cClose(");
}

void DummyIoProvider::i2cWriteByte(unsigned int handle, uint8_t data){
    Logger::logDebugFrom("DummyIoProvider", "i2cWriteByte(" + std::to_string(handle) + ", " + std::to_string(data) + ")");
}

uint8_t DummyIoProvider::i2cReadByte(unsigned int handle){
    Logger::logDebugFrom("DummyIoProvider", "i2cReadByte(" + std::to_string(handle) + ")");
    return 0;
}

void DummyIoProvider::i2cWriteBytes(unsigned int handle, uint8_t *buf, unsigned int count){
    Logger::logDebugFrom("DummyIoProvider", "i2cWriteBytes(" + std::to_string(handle) + ", buf, " + std::to_string(count) + ")");
}

void DummyIoProvider::i2cReadBytes(unsigned int handle, uint8_t *buf, unsigned int count){
    Logger::logDebugFrom("DummyIoProvider", "i2cReadBytes(" + std::to_string(handle) + ", buf, " + std::to_string(count) + ")");
}

void DummyIoProvider::i2cWriteReg8(unsigned int handle, uint8_t reg, uint8_t value){
    Logger::logDebugFrom("DummyIoProvider", "i2cWriteReg8(" + std::to_string(handle) + ", " + std::to_string(reg) + ", " + std::to_string(value) + ")");
}

uint8_t DummyIoProvider::i2cReadReg8(unsigned int handle, uint8_t reg){
    Logger::logDebugFrom("DummyIoProvider", "i2cReadReg8(" + std::to_string(handle) + ", " + std::to_string(reg) + ")");
    return 0;
}

void DummyIoProvider::i2cWriteReg16(unsigned int handle, uint8_t reg, uint16_t value){
    Logger::logDebugFrom("DummyIoProvider", "i2cWriteReg16(" + std::to_string(handle) + ", " + std::to_string(reg) + ", " + std::to_string(value) + ")");
}

uint16_t DummyIoProvider::i2cReadReg16(unsigned int handle, uint8_t reg){
    Logger::logDebugFrom("DummyIoProvider", "i2cReadReg16(" + std::to_string(handle) + ", " + std::to_string(reg) + ")");
    return 0;
}


////////////////////////////////////////////////////////////////////////
/// SPI
////////////////////////////////////////////////////////////////////////

unsigned int DummyIoProvider::spiOpen(unsigned int bus, unsigned int channel, unsigned int baud){
    Logger::logDebugFrom("DummyIoProvider", "spiOpen(" + std::to_string(bus) + ", " + std::to_string(channel) + ", " + std::to_string(baud) + ")");
    return 0;
}

void DummyIoProvider::spiClose(unsigned int handle){
    Logger::logDebugFrom("DummyIoProvider", "spiClose(" + std::to_string(handle) + ")");
}

void DummyIoProvider::spiWrite(unsigned int handle, char *buf, unsigned int count){
    Logger::logDebugFrom("DummyIoProvider", "spiWrite(" + std::to_string(handle) + ", buf, " + std::to_string(count) + ")");
}

void DummyIoProvider::spiRead(unsigned int handle, char *buf, unsigned int count){
    Logger::logDebugFrom("DummyIoProvider", "spiRead(" + std::to_string(handle) + ", buf, " + std::to_string(count) + ")");
}


////////////////////////////////////////////////////////////////////////
/// UART
////////////////////////////////////////////////////////////////////////

unsigned int DummyIoProvider::uartOpen(char *port, unsigned int baud){
    Logger::logDebugFrom("DummyIoProvider", "uartOpen(" + std::string(port) + ", " + std::to_string(baud) + ")");
    return 0;
}

void DummyIoProvider::uartClose(unsigned int handle){
    Logger::logDebugFrom("DummyIoProvider", "uartClose(" + std::to_string(handle) + ")");
}

unsigned int DummyIoProvider::uartAvailable(unsigned int handle){
    Logger::logDebugFrom("DummyIoProvider", "uartAvailable(" + std::to_string(handle) + ")");
    return 0;
}

void DummyIoProvider::uartWrite(unsigned int handle, char* buf, unsigned int count){
    Logger::logDebugFrom("DummyIoProvider", "uartWrite(" + std::to_string(handle) + ", buf, " + std::to_string(count)  + ")");
}

void DummyIoProvider::uartRead(unsigned int handle, char *buf, unsigned int count){
    Logger::logDebugFrom("DummyIoProvider", "uartRead(" + std::to_string(handle) + ", buf, " + std::to_string(count) + ")");
}

void DummyIoProvider::uartWriteByte(unsigned int handle, uint8_t b){
    Logger::logDebugFrom("DummyIoProvider", "uartWriteByte(" + std::to_string(handle) + ", " + std::to_string(b) + ")");
}

uint8_t DummyIoProvider::uartReadByte(unsigned int handle){
    Logger::logDebugFrom("DummyIoProvider", "uartReadByte(" + std::to_string(handle) + ")");
    return 0;
}
