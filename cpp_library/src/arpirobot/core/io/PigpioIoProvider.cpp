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


#if defined(__linux__) && defined(__arm__)


#include <arpirobot/core/io/PigpioIoProvider.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/io/exceptions.hpp>

#include <pigpio.h>

using namespace arpirobot;


PigpioIoProvider::PigpioIoProvider() : IoProvider(){

    // This configures pigpio to use PWM clock not PCM clock for DMA transfers
    // This prevents use of pigpio from breaking audio playback through the I2S interface
    gpioCfgClock(5, 0, 0);
    int res = gpioInitialise();
    handlePigpioError(res, false);
    Logger::logInfoFrom("PigpioIoProvider", "IO Provider initialized.");
}
        
PigpioIoProvider::~PigpioIoProvider(){
    gpioTerminate();
    Logger::logInfoFrom("PigpioIoProvider", "IO Provider terminated.");
}


////////////////////////////////////////////////////////////////////////
/// GPIO & PWM
////////////////////////////////////////////////////////////////////////
void PigpioIoProvider::gpioMode(unsigned int pin, unsigned int mode){
    int res = ::gpioSetMode(pin, toPigpioMode(mode));
    handlePigpioError(res, false);
}

void PigpioIoProvider::gpioWrite(unsigned int pin, unsigned int state){
    int res = ::gpioWrite(pin, toPigpioState(state));
    handlePigpioError(res, false);
}

unsigned int PigpioIoProvider::gpioRead(unsigned int pin){
    int res = ::gpioRead(pin);
    handlePigpioError(res, false);
    return fromPigpioState(res);
}

void PigpioIoProvider::gpioSetPwmFrequency(unsigned int pin, unsigned int frequency){
    int res = ::gpioSetPWMfrequency(pin, frequency);
    handlePigpioError(res, false);
}

void PigpioIoProvider::gpioPwm(unsigned int pin, unsigned int value){
    int res = ::gpioPWM(pin, value);
    handlePigpioError(pin, false);
}


////////////////////////////////////////////////////////////////////////
/// I2C
////////////////////////////////////////////////////////////////////////
unsigned int PigpioIoProvider::i2cOpen(unsigned int bus, unsigned int address){
    int res = ::i2cOpen(bus, address, 0);
    handlePigpioError(res, false);
    return res;
}

void PigpioIoProvider::i2cClose(unsigned int handle){
    int res = ::i2cClose(handle);
    handlePigpioError(res, false);
}

void PigpioIoProvider::i2cWriteByte(unsigned int handle, uint8_t data){
    int res = ::i2cWriteByte(handle, data);
    handlePigpioError(res, true);
}

uint8_t PigpioIoProvider::i2cReadByte(unsigned int handle){
    int res = ::i2cReadByte(handle);
    handlePigpioError(res, false);
    return res;
}

void PigpioIoProvider::i2cWriteBytes(unsigned int handle, char *buf, unsigned int count){
    int res = ::i2cWriteDevice(handle, buf, count);
    handlePigpioError(res, true);
}

unsigned int PigpioIoProvider::i2cReadBytes(unsigned int handle, char *buf, unsigned int count){
    int res = ::i2cReadDevice(handle, buf, count);
    handlePigpioError(res, false);
    return res;
}

void PigpioIoProvider::i2cWriteReg8(unsigned int handle, uint8_t reg, uint8_t value){
    int res = ::i2cWriteByteData(handle, reg, value);
    handlePigpioError(res, true);
}

uint8_t PigpioIoProvider::i2cReadReg8(unsigned int handle, uint8_t reg){
    int res = ::i2cReadByteData(handle, reg);
    handlePigpioError(res, false);
    return res;
}

void PigpioIoProvider::i2cWriteReg16(unsigned int handle, uint8_t reg, uint16_t value){
    int res = ::i2cWriteWordData(handle, reg, value);
    handlePigpioError(res, true);
}

uint16_t PigpioIoProvider::i2cReadReg16(unsigned int handle, uint8_t reg){
    int res = ::i2cReadWordData(handle, reg);
    handlePigpioError(res, false);
    return res;
}


////////////////////////////////////////////////////////////////////////
/// SPI
////////////////////////////////////////////////////////////////////////

// Bus = spi bus 
// Channel = which builtin CS pin
unsigned int PigpioIoProvider::spiOpen(unsigned int bus, unsigned int channel, unsigned int baud){
    if(bus > 1 || bus < 0){
        throw InvalidParameterException();
    }
    int flags = 0 | (bus << 8);
    int res = ::spiOpen(channel, baud, flags);
    handlePigpioError(res, false);
    return res;
}

void PigpioIoProvider::spiClose(unsigned int handle){
    int res = ::spiClose(handle);
    handlePigpioError(res, false);
}

void PigpioIoProvider::spiWrite(unsigned int handle, char *buf, unsigned int count){
    int res = ::spiWrite(handle, buf, count);
    handlePigpioError(res, true);
}

unsigned int PigpioIoProvider::spiRead(unsigned int handle, char *buf, unsigned int count){
    int res = ::spiRead(handle, buf, count);
    handlePigpioError(res, false);
    return res;
}


////////////////////////////////////////////////////////////////////////
/// UART
////////////////////////////////////////////////////////////////////////

unsigned int PigpioIoProvider::uartOpen(char *port, unsigned int baud){
    int res = ::serOpen(port, baud, 0);
    handlePigpioError(res, false);
    return res;
}

void PigpioIoProvider::uartClose(unsigned int handle){
    int res = ::serClose(handle);
    handlePigpioError(res, false);
}

unsigned int PigpioIoProvider::uartAvailable(unsigned int handle){
    int res = ::serDataAvailable(handle);
    handlePigpioError(res, false);
    return res;
}

void PigpioIoProvider::uartWrite(unsigned int handle, char* buf, unsigned int count){
    int res = ::serWrite(handle, buf, count);
    handlePigpioError(res, true);
}

unsigned int PigpioIoProvider::uartRead(unsigned int handle, char *buf, unsigned int count){
    int res = ::serRead(handle, buf, count);
    if(res == PI_SER_READ_NO_DATA) return 0;
    handlePigpioError(res, false);
    return res;
}

void PigpioIoProvider::uartWriteByte(unsigned int handle, uint8_t b){
    int res = ::serWriteByte(handle, b);
    handlePigpioError(res, true);
}

uint8_t PigpioIoProvider::uartReadByte(unsigned int handle){
    int res = ::serReadByte(handle);
    if(res == PI_SER_READ_NO_DATA) return 0;
    handlePigpioError(res, false);
    return res;
}



void PigpioIoProvider::handlePigpioError(int ec, bool opIsWrite){
    switch(ec){
    case PI_INIT_FAILED:
        throw std::runtime_error("Failed to initialize pigpio library");
    case PI_BAD_GPIO:
    case PI_BAD_USER_GPIO:
        throw InvalidPinException();
    case PI_BAD_LEVEL:
    case PI_BAD_DUTYCYCLE:
    case PI_BAD_I2C_BUS:
    case PI_BAD_I2C_ADDR:
    case PI_BAD_FLAGS:
    case PI_BAD_PARAM:
    case PI_BAD_SPI_CHANNEL:
    case PI_BAD_SPI_SPEED:
    case PI_NO_AUX_SPI:
        throw InvalidParameterException();
    case PI_NO_HANDLE:
    case PI_BAD_HANDLE:
        throw BadHandleException();
    case PI_I2C_OPEN_FAILED:
    case PI_SER_OPEN_FAILED:
    case PI_SPI_OPEN_FAILED:
        throw OpenFailedException();
    case PI_I2C_WRITE_FAILED:
    case PI_SER_WRITE_FAILED:
        throw WriteFailedException();
    case PI_I2C_READ_FAILED:
    case PI_SER_READ_FAILED:
        throw ReadFailedException();
    case PI_SPI_XFER_FAILED:
    case PI_BAD_SPI_COUNT:
        if(opIsWrite){
            throw WriteFailedException();
        }else{
            throw ReadFailedException();
        }
    }
}

int PigpioIoProvider::toPigpioMode(int mode){
    if(mode == GPIO_OUT) return PI_OUTPUT;
    if(mode == GPIO_IN) return PI_INPUT;
    throw InvalidParameterException();
}
        
int PigpioIoProvider::fromPigpioMode(int mode){
    if(mode == PI_INPUT) return GPIO_IN;
    if(mode == PI_OUTPUT) return GPIO_OUT;
    throw InvalidParameterException();
}

int PigpioIoProvider::toPigpioState(int state){
    if(state == GPIO_HIGH) return PI_HIGH;
    if(state == GPIO_LOW) return PI_LOW;
    throw InvalidParameterException();
}

int PigpioIoProvider::fromPigpioState(int state){
    if(state == PI_HIGH) return GPIO_HIGH;
    if(state == PI_LOW) return GPIO_LOW;
    throw InvalidParameterException();
}

#endif
