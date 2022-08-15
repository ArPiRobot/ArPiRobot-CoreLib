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

#ifdef HAS_LIBSOC


#include <arpirobot/core/io/LibsocIoProvider.hpp>
#include <arpirobot/core/io/SerialIoProvider.hpp>
#include <arpirobot/core/log/Logger.hpp>

using namespace arpirobot;


LibsocIoProvider::LibsocIoProvider(){
#ifdef HAS_SERIAL
    Logger::logInfoFrom("LibsocIoProvider", "Wrapping LibsocIoProvider for UART support.");
    uartProvider = new SerialIoProvider(true);
#else
    Logger::logWarning("Using libsoc without serial. UART will not function.");
#endif

    // TODO: Actually initialize libsoc GPIO, PWM, SPI, and I2C

    Logger::logInfoFrom("LibsocIoProvider", "IO Provider initialized.");

}

LibsocIoProvider::~LibsocIoProvider(){
    if(uartProvider != nullptr){
        delete uartProvider;
    }

    // TODO: Actually terminate libsoc GPIO, PWM, SPI, and I2C

    Logger::logInfoFrom("LibsocIoProvider", "IO Provider terminated.");
}

////////////////////////////////////////////////////////////////////////
/// GPIO & PWM
////////////////////////////////////////////////////////////////////////
void LibsocIoProvider::gpioMode(unsigned int pin, unsigned int mode){

}

void LibsocIoProvider::gpioWrite(unsigned int pin, unsigned int state){

}

unsigned int LibsocIoProvider::gpioRead(unsigned int pin){
    return 0;
}

void LibsocIoProvider::gpioSetPwmFrequency(unsigned int pin, unsigned int frequency){

}

unsigned int LibsocIoProvider::gpioGetPwmFrequency(unsigned int pin){
    return 0;
}

void LibsocIoProvider::gpioPwm(unsigned int pin, unsigned int value){

}


////////////////////////////////////////////////////////////////////////
/// I2C
////////////////////////////////////////////////////////////////////////
unsigned int LibsocIoProvider::i2cOpen(unsigned int bus, unsigned int address){
    return 0;
}

void LibsocIoProvider::i2cClose(unsigned int handle){

}

void LibsocIoProvider::i2cWriteByte(unsigned int handle, uint8_t data){

}

uint8_t LibsocIoProvider::i2cReadByte(unsigned int handle){
    return 0;
}

void LibsocIoProvider::i2cWriteBytes(unsigned int handle, char *buf, unsigned int count){

}

unsigned int LibsocIoProvider::i2cReadBytes(unsigned int handle, char *buf, unsigned int count){
    return 0;
}

void LibsocIoProvider::i2cWriteReg8(unsigned int handle, uint8_t reg, uint8_t value){

}

uint8_t LibsocIoProvider::i2cReadReg8(unsigned int handle, uint8_t reg){
    return 0;
}

void LibsocIoProvider::i2cWriteReg16(unsigned int handle, uint8_t reg, uint16_t value){

}

uint16_t LibsocIoProvider::i2cReadReg16(unsigned int handle, uint8_t reg){
    return 0;
}


////////////////////////////////////////////////////////////////////////
/// SPI
////////////////////////////////////////////////////////////////////////

unsigned int LibsocIoProvider::spiOpen(unsigned int bus, unsigned int channel, unsigned int baud, unsigned int mode){
    return 0;
}

void LibsocIoProvider::spiClose(unsigned int handle){

}

void LibsocIoProvider::spiWrite(unsigned int handle, char *buf, unsigned int count){

}

unsigned int LibsocIoProvider::spiRead(unsigned int handle, char *buf, unsigned int count){
    return 0;
}


////////////////////////////////////////////////////////////////////////
/// UART
////////////////////////////////////////////////////////////////////////

unsigned int LibsocIoProvider::uartOpen(char *port, unsigned int baud){
    return 0;
}

void LibsocIoProvider::uartClose(unsigned int handle){
    
}

unsigned int LibsocIoProvider::uartAvailable(unsigned int handle){
    return 0;
}

void LibsocIoProvider::uartWrite(unsigned int handle, char* buf, unsigned int count){
    
}

unsigned int LibsocIoProvider::uartRead(unsigned int handle, char *buf, unsigned int count){
    return 0;
}

void LibsocIoProvider::uartWriteByte(unsigned int handle, uint8_t b){
    
}

uint8_t LibsocIoProvider::uartReadByte(unsigned int handle){
    return 0;
}

#endif // HAS_LIBSOC