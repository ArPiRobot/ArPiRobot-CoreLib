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
    Logger::logDebugFrom("DummyIoProvider", "");
}

unsigned int DummyIoProvider::gpioRead(unsigned int pin){
    Logger::logDebugFrom("DummyIoProvider", "");
}

void DummyIoProvider::gpioSetPwmFrequency(unsigned int pin, unsigned int frequency){
    Logger::logDebugFrom("DummyIoProvider", "");
}

unsigned int DummyIoProvider::gpioPwm(unsigned int pin, unsigned int value){
    Logger::logDebugFrom("DummyIoProvider", "");
}


////////////////////////////////////////////////////////////////////////
/// I2C
////////////////////////////////////////////////////////////////////////
unsigned int DummyIoProvider::i2cOpen(unsigned int bus, unsigned int address){
    Logger::logDebugFrom("DummyIoProvider", "");
}

void DummyIoProvider::i2cClose(unsigned int handle){
    Logger::logDebugFrom("DummyIoProvider", "");
}

unsigned int DummyIoProvider::i2cWriteByte(unsigned int handle, uint8_t data){
    Logger::logDebugFrom("DummyIoProvider", "");
}

uint8_t DummyIoProvider::i2cReadByte(unsigned int handle){
    Logger::logDebugFrom("DummyIoProvider", "");
}

uint8_t DummyIoProvider::i2cWriteReg8(unsigned int handle, uint8_t reg, uint8_t value){
    Logger::logDebugFrom("DummyIoProvider", "");
}

uint8_t DummyIoProvider::i2cReadReg8(unsigned int handle, uint8_t reg){
    Logger::logDebugFrom("DummyIoProvider", "");
}

uint16_t DummyIoProvider::i2cWriteReg16(unsigned int handle, uint8_t reg, uint16_t value){
    Logger::logDebugFrom("DummyIoProvider", "");
}

uint16_t DummyIoProvider::i2cReadReg16(unsigned int handle, uint8_t reg){
    Logger::logDebugFrom("DummyIoProvider", "");
}


////////////////////////////////////////////////////////////////////////
/// SPI
////////////////////////////////////////////////////////////////////////

unsigned int DummyIoProvider::spiOpen(unsigned int bus, unsigned int channel){
    Logger::logDebugFrom("DummyIoProvider", "");
}

void DummyIoProvider::spiClose(unsigned int handle){
    Logger::logDebugFrom("DummyIoProvider", "");
}

void DummyIoProvider::spiWrite(unsigned int handle, uint8_t *buf, unsigned int count){
    Logger::logDebugFrom("DummyIoProvider", "");
}

void DummyIoProvider::spiRead(unsigned int handle, uint8_t *buf, unsigned int count){
    Logger::logDebugFrom("DummyIoProvider", "");
}


////////////////////////////////////////////////////////////////////////
/// UART
////////////////////////////////////////////////////////////////////////

unsigned int DummyIoProvider::uartOpen(const char *port, unsigned int buad){
    Logger::logDebugFrom("DummyIoProvider", "");
}

void DummyIoProvider::uartClose(unsigned int handle){
    Logger::logDebugFrom("DummyIoProvider", "");
}

unsigned int DummyIoProvider::uartAvailable(unsigned int handle){
    Logger::logDebugFrom("DummyIoProvider", "");
}

void DummyIoProvider::uartWrite(unsigned int handle, const char* buf, unsigned int count){
    Logger::logDebugFrom("DummyIoProvider", "");
}

void DummyIoProvider::uartRead(unsigned int handle, const char *buf, unsigned int count){
    Logger::logDebugFrom("DummyIoProvider", "");
}
