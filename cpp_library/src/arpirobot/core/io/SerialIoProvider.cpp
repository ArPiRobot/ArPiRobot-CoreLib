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


#include <arpirobot/core/io/SerialIoProvider.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/io/exceptions.hpp>

using namespace arpirobot;

#ifdef HAS_SERIAL

SerialIoProvider::SerialIoProvider() : IoProvider(){
    Logger::logWarning("The serial IO provider has been started. This provider CANNOT be used to" 
            "interact with MOST devices. It only supports UART operations.");
    Logger::logInfoFrom("SerialIoProvider", "IO Provider initialized.");
}
        
SerialIoProvider::~SerialIoProvider(){
    for(auto &it : handleMap){
        it.second->close();
        delete it.second;
    }

    Logger::logInfoFrom("SerialIoProvider", "IO Provider terminated.");
}

////////////////////////////////////////////////////////////////////////
/// GPIO & PWM
////////////////////////////////////////////////////////////////////////
void SerialIoProvider::gpioMode(unsigned int pin, unsigned int mode){

}

void SerialIoProvider::gpioWrite(unsigned int pin, unsigned int state){

}

unsigned int SerialIoProvider::gpioRead(unsigned int pin){
    return 0;
}

void SerialIoProvider::gpioSetPwmFrequency(unsigned int pin, unsigned int frequency){

}

unsigned int SerialIoProvider::gpioGetPwmFrequency(unsigned int pin){
    return 0;
}

void SerialIoProvider::gpioPwm(unsigned int pin, unsigned int value){

}


////////////////////////////////////////////////////////////////////////
/// I2C
////////////////////////////////////////////////////////////////////////
unsigned int SerialIoProvider::i2cOpen(unsigned int bus, unsigned int address){
    return 0;
}

void SerialIoProvider::i2cClose(unsigned int handle){

}

void SerialIoProvider::i2cWriteByte(unsigned int handle, uint8_t data){

}

uint8_t SerialIoProvider::i2cReadByte(unsigned int handle){
    return 0;
}

void SerialIoProvider::i2cWriteBytes(unsigned int handle, char *buf, unsigned int count){

}

unsigned int SerialIoProvider::i2cReadBytes(unsigned int handle, char *buf, unsigned int count){
    return 0;
}

void SerialIoProvider::i2cWriteReg8(unsigned int handle, uint8_t reg, uint8_t value){

}

uint8_t SerialIoProvider::i2cReadReg8(unsigned int handle, uint8_t reg){
    return 0;
}

void SerialIoProvider::i2cWriteReg16(unsigned int handle, uint8_t reg, uint16_t value){

}

uint16_t SerialIoProvider::i2cReadReg16(unsigned int handle, uint8_t reg){
    return 0;
}


////////////////////////////////////////////////////////////////////////
/// SPI
////////////////////////////////////////////////////////////////////////

unsigned int SerialIoProvider::spiOpen(unsigned int bus, unsigned int channel, unsigned int baud, unsigned int mode){
    return 0;
}

void SerialIoProvider::spiClose(unsigned int handle){

}

void SerialIoProvider::spiWrite(unsigned int handle, char *buf, unsigned int count){

}

unsigned int SerialIoProvider::spiRead(unsigned int handle, char *buf, unsigned int count){
    return 0;
}


////////////////////////////////////////////////////////////////////////
/// UART
////////////////////////////////////////////////////////////////////////

unsigned int SerialIoProvider::uartOpen(char *port, unsigned int baud){
    serial::Serial *serial = new serial::Serial(port, baud);
    if(!serial->isOpen()){
        delete serial;
        throw OpenFailedException();
    }
    int handle = currentHandle++;
    handleMap[handle] = serial;
    return handle;
}

void SerialIoProvider::uartClose(unsigned int handle){
    if(handleMap.find(handle) != handleMap.end()){
        handleMap[handle]->close();
        delete handleMap[handle];
        handleMap.erase(handle);
    }else{
        throw BadHandleException();
    }
}

unsigned int SerialIoProvider::uartAvailable(unsigned int handle){
    if(handleMap.find(handle) != handleMap.end()){
        return handleMap[handle]->available();
    }else{
        throw BadHandleException();
    }
}

void SerialIoProvider::uartWrite(unsigned int handle, char* buf, unsigned int count){
    if(handleMap.find(handle) != handleMap.end()){
        handleMap[handle]->write((uint8_t*)buf, count);
    }else{
        throw BadHandleException();
    }
}

unsigned int SerialIoProvider::uartRead(unsigned int handle, char *buf, unsigned int count){
    if(handleMap.find(handle) != handleMap.end()){
        return handleMap[handle]->read((uint8_t*)buf, count);
    }else{
        throw BadHandleException();
    }
}

void SerialIoProvider::uartWriteByte(unsigned int handle, uint8_t b){
    if(handleMap.find(handle) != handleMap.end()){
        handleMap[handle]->write(&b, 1);
    }else{
        throw BadHandleException();
    }
}

uint8_t SerialIoProvider::uartReadByte(unsigned int handle){
    if(handleMap.find(handle) != handleMap.end()){
        uint8_t b;
        handleMap[handle]->read(&b, 1);
        return b;
    }else{
        throw BadHandleException();
    }
}


#endif // HAS_SERIAL