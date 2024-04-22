/*
 * Copyright 2024 Marcus Behel
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

#if HAS_LGPIO

#include <arpirobot/core/io/LgpioIoProvider.hpp>
#include <arpirobot/core/io/exceptions.hpp>
#include <arpirobot/core/log/Logger.hpp>

#include <iostream>

using namespace arpirobot;


LgpioIoProvider::LgpioIoProvider(){
    // Open all GPIO chips
    unsigned int chip = 0;
    int handle, res;
    lgChipInfo_t info;
    while(chip < 99){   // 99 chips is just a failsafe exit condition
        handle = lgGpiochipOpen(chip);
        if(handle == LG_CANNOT_OPEN_CHIP)
            break;
        handleLgpioError(handle, false);
        chipHandles[chip] = handle;
        res = lgGpioGetChipInfo(handle, &info);
        handleLgpioError(res, false);
        chipLineCounts[chip] = info.lines;
        chip++;
    }
    Logger::logInfoFrom("LgpioIoProvider", "IO Provider initialized.");
}

LgpioIoProvider::~LgpioIoProvider(){
    for(auto pin : claimedGpios){
        auto chipAndLine = pinToChipAndLine(pin);
        unsigned int chip = chipAndLine.first;
        unsigned int line = chipAndLine.second;
        lgGpioFree(chipHandles[chip], line);
    }
    while(i2cHandles.size() > 0){
        i2cClose(*i2cHandles.begin());
    }
    while(spiHandles.size() > 0){
        spiClose(*spiHandles.begin());
    }
    while(uartHandles.size() > 0){
        uartClose(*uartHandles.begin());
    }
    for(auto &it : chipHandles){
        lgGpiochipClose(it.second);
    }
    Logger::logInfoFrom("LgpioIoProvider", "IO Provider terminated.");
}


////////////////////////////////////////////////////////////////////////
/// GPIO & PWM
////////////////////////////////////////////////////////////////////////
void LgpioIoProvider::gpioMode(unsigned int pin, unsigned int mode) {
    auto chipAndLine = pinToChipAndLine(pin);
    unsigned int chip = chipAndLine.first;
    unsigned int line = chipAndLine.second;
    int res;
    if(claimedGpios.find(pin) != claimedGpios.end()){
        res = lgGpioFree(chipHandles[chip], line);
        handleLgpioError(res, false);
        claimedGpios.erase(pin);
    }
    if(mode == GPIO_OUT){
        res = lgGpioClaimOutput(chipHandles[chip], 0, line, 0);
        handleLgpioError(res, false);
        claimedGpios.insert(pin);
    }else if(mode == GPIO_IN){
        res = lgGpioClaimInput(chipHandles[chip], 0, line);
        handleLgpioError(res, false);
        claimedGpios.insert(pin);
    }else{
        throw InvalidParameterException();
    }
}

void LgpioIoProvider::gpioWrite(unsigned int pin, unsigned int state) {
    auto chipAndLine = pinToChipAndLine(pin);
    unsigned int chip = chipAndLine.first;
    unsigned int line = chipAndLine.second;
    int res;
    if(state == GPIO_HIGH)
        res = lgGpioWrite(chipHandles[chip], line, 1);
    else if(state == GPIO_LOW)
        res = lgGpioWrite(chipHandles[chip], line, 0);
    else
        throw InvalidParameterException();
    handleLgpioError(res, true);
}

unsigned int LgpioIoProvider::gpioRead(unsigned int pin) {
    auto chipAndLine = pinToChipAndLine(pin);
    unsigned int chip = chipAndLine.first;
    unsigned int line = chipAndLine.second;
    int res = lgGpioRead(chipHandles[chip], line);
    handleLgpioError(res, false);
    return (res == 1) ? GPIO_HIGH : GPIO_LOW;
}

void LgpioIoProvider::gpioSetPwmFrequency(unsigned int pin, unsigned int frequency) {
    if(frequency < 1 || frequency > 10000)
        throw InvalidParameterException();
    pwmFreqs[pin] = frequency;
}

unsigned int LgpioIoProvider::gpioGetPwmFrequency(unsigned int pin) {
    if(pwmFreqs.find(pin) != pwmFreqs.end())
        return pwmFreqs[pin];
    return 125;
}

void LgpioIoProvider::gpioPwm(unsigned int pin, unsigned int value) {
    auto chipAndLine = pinToChipAndLine(pin);
    unsigned int chip = chipAndLine.first;
    unsigned int line = chipAndLine.second;
    unsigned int freq = 125;
    if(pwmFreqs.find(pin) != pwmFreqs.end())
        freq = pwmFreqs[pin];
    // Duty cycle is expected as percent (0-100) but provided as range 0-255
    float dc = ((float)value / 255.0f) * 100.0f;
    int res = lgTxPwm(chipHandles[chip], line, freq, dc, 0, 0);
    handleLgpioError(res, true);
}

////////////////////////////////////////////////////////////////////////
/// I2C
////////////////////////////////////////////////////////////////////////
unsigned int LgpioIoProvider::i2cOpen(unsigned int bus, unsigned int address) {
    int res = lgI2cOpen(bus, address, 0);
    handleLgpioError(res, false);
    i2cHandles.insert(res);
    return res;
}

void LgpioIoProvider::i2cClose(unsigned int handle) {
    int res = lgI2cClose(handle);
    handleLgpioError(res, false);
    i2cHandles.erase(handle);
}

void LgpioIoProvider::i2cWriteByte(unsigned int handle, uint8_t data) {
    int res = lgI2cWriteByte(handle, data);
    handleLgpioError(res, true);
}

uint8_t LgpioIoProvider::i2cReadByte(unsigned int handle) {
    int res = lgI2cReadByte(handle);
    handleLgpioError(res, false);
    return (uint8_t) res;
}

void LgpioIoProvider::i2cWriteBytes(unsigned int handle, char *buf, unsigned int count) {
    int res = lgI2cWriteDevice(handle, buf, count);
    handleLgpioError(res, true);
}

unsigned int LgpioIoProvider::i2cReadBytes(unsigned int handle, char *buf, unsigned int count) {
    int res = lgI2cReadDevice(handle, buf, count);
    handleLgpioError(res, false);
    return res;
}

void LgpioIoProvider::i2cWriteReg8(unsigned int handle, uint8_t reg, uint8_t value) {
    int res = lgI2cWriteByteData(handle, reg, value);
    handleLgpioError(res, true);
}

uint8_t LgpioIoProvider::i2cReadReg8(unsigned int handle, uint8_t reg) {
    int res = lgI2cReadByteData(handle, reg);
    handleLgpioError(res, false);
    return (uint8_t) res;
}

void LgpioIoProvider::i2cWriteReg16(unsigned int handle, uint8_t reg, uint16_t value) {
    int res = lgI2cWriteWordData(handle, reg, value);
    handleLgpioError(res, true);
}

uint16_t LgpioIoProvider::i2cReadReg16(unsigned int handle, uint8_t reg) {
    int res = lgI2cReadWordData(handle, reg);
    handleLgpioError(res, false);
    return (uint16_t)res;
}

////////////////////////////////////////////////////////////////////////
/// SPI
////////////////////////////////////////////////////////////////////////
unsigned int LgpioIoProvider::spiOpen(unsigned int bus, unsigned int channel, unsigned int baud, unsigned int mode) {
    int res = lgSpiOpen(bus, channel, baud, mode);
    handleLgpioError(res, false);
    spiHandles.insert(res);
    return res;
}

void LgpioIoProvider::spiClose(unsigned int handle) {
    int res = lgSpiClose(handle);
    handleLgpioError(res, false);
    spiHandles.erase(handle);
}

void LgpioIoProvider::spiWrite(unsigned int handle, char *buf, unsigned int count) {
    int res = lgSpiWrite(handle, buf, count);
    handleLgpioError(res, true);
}

unsigned int LgpioIoProvider::spiRead(unsigned int handle, char *buf, unsigned int count) {
    int res = lgSpiRead(handle, buf, count);
    handleLgpioError(res, false);
    return res;
}

////////////////////////////////////////////////////////////////////////
/// UART
////////////////////////////////////////////////////////////////////////
unsigned int LgpioIoProvider::uartOpen(char *port, unsigned int baud) {
    int res = lgSerialOpen(port, baud, 0);
    handleLgpioError(res, false);
    uartHandles.insert(res);
    return res;
}

void LgpioIoProvider::uartClose(unsigned int handle) {
    int res = lgSerialClose(handle);
    handleLgpioError(res, false);
    uartHandles.erase(handle);
}

unsigned int LgpioIoProvider::uartAvailable(unsigned int handle) {
    int res = lgSerialDataAvailable(handle);
    handleLgpioError(res, false);
    return res;
}

void LgpioIoProvider::uartWrite(unsigned int handle, char* buf, unsigned int count) {
    int res = lgSerialWrite(handle, buf, count);
    handleLgpioError(res, true);
}

unsigned int LgpioIoProvider::uartRead(unsigned int handle, char *buf, unsigned int count) {
    int res = lgSerialRead(handle, buf, count);
    handleLgpioError(res, false);
    return res;
}

void LgpioIoProvider::uartWriteByte(unsigned int handle, uint8_t b) {
    int res = lgSerialWriteByte(handle, b);
    handleLgpioError(res, true);
}

uint8_t LgpioIoProvider::uartReadByte(unsigned int handle) {
    int res = lgSerialReadByte(handle);
    handleLgpioError(res, false);
    return res;
}



std::pair<unsigned int, unsigned int> LgpioIoProvider::pinToChipAndLine(unsigned int pin){
    unsigned int chip = 0;
    unsigned int line = 0;
    while(pin >= chipLineCounts[chip]){
        pin -= chipLineCounts[chip];
        chip++;
        if(chip >= chipLineCounts.size())
            throw InvalidPinException();
    }
    line = pin;
    return std::pair<unsigned int, unsigned int>(chip, pin);
}

unsigned int LgpioIoProvider::chipAndLineToPin(unsigned int chip, unsigned int line){
    unsigned int pin = line;
    while(chip > 0){
        pin += chipLineCounts[chip - 1];
        chip--;
    }
    return pin;
}

void LgpioIoProvider::handleLgpioError(int ec, bool opIsWrite){
    switch(ec){
    case LG_INIT_FAILED:
        throw std::runtime_error("Failed to initialize lgpio library");
    case LG_NO_HANDLE:
    case LG_BAD_HANDLE:
        throw BadHandleException();
    case LG_NOT_PERMITTED:
    case LG_SOME_PERMITTED:
        throw std::runtime_error("lgpio error: permission denied");
    case LG_GPIO_IN_USE:
    case LG_I2C_OPEN_FAILED:
    case LG_SERIAL_OPEN_FAILED:
    case LG_SPI_OPEN_FAILED:
    case LG_NOT_A_GPIOCHIP:
    case LG_GPIO_BUSY:
        throw OpenFailedException();
    case LG_BAD_MICROS:
    case LG_BAD_I2C_BUS:
    case LG_BAD_I2C_ADDR:
    case LG_BAD_SPI_CHANNEL:
    case LG_BAD_I2C_FLAGS:
    case LG_BAD_SPI_FLAGS:
    case LG_BAD_SERIAL_FLAGS:
    case LG_BAD_SPI_SPEED:
    case LG_BAD_SERIAL_DEVICE:
    case LG_BAD_SERIAL_SPEED:
    case LG_BAD_FILE_PARAM:
    case LG_BAD_I2C_PARAM:
    case LG_BAD_SERIAL_PARAM:
    case LG_BAD_SPI_COUNT:
    case LG_BAD_POINTER:
    case LG_TOO_MANY_SEGS:
    case LG_GPIO_NOT_ALLOCATED:
    case LG_BAD_PWM_MICROS:
    case LG_BAD_GPIOCHIP:
    case LG_BAD_PWM_FREQ:
    case LG_BAD_PWM_DUTY:
    case LG_GPIO_NOT_AN_OUTPUT:
        throw InvalidParameterException();
    case LG_I2C_WRITE_FAILED:
    case LG_SERIAL_WRITE_FAILED:
    case LG_BAD_I2C_WLEN:
    case LG_BAD_WRITE:
        throw WriteFailedException();
    case LG_I2C_READ_FAILED:
    case LG_SERIAL_READ_FAILED:
    case LG_BAD_I2C_RLEN:
    case LG_BAD_READ:
        throw ReadFailedException();
    case LG_SPI_XFER_FAILED:
    case LG_BAD_I2C_SEG:
    case LG_BAD_SMBUS_CMD:
    case LG_BAD_I2C_CMD:
    case LG_SPI_IOCTL_FAILED:
    case LG_BAD_CHIPINFO_IOCTL:
    case LG_NO_PERMISSIONS:
        if(opIsWrite) throw WriteFailedException();
        else throw ReadFailedException();
    case LG_TX_QUEUE_FULL:
        throw std::runtime_error("Out of slots in PWM queue (too many PWMs, lgpio can't handle more)");
    default:
        if(ec < 0){
            Logger::logWarningFrom("LgpioIoProvider", "Got unhandled error code ");
            std::cout << "STUPID THINGS: " << ec << std::endl;
        }
        return;
    }
}

#endif // HAS_LGPIO