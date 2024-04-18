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


#pragma once

#ifdef HAS_LGPIO

#include <arpirobot/core/io/IoProvider.hpp>
#include <lgpio.h>
#include <utility>
#include <unordered_map>
#include <unordered_set>

namespace arpirobot{
    class LgpioIoProvider : public IoProvider{
    protected:

        LgpioIoProvider();

        ~LgpioIoProvider();

        ////////////////////////////////////////////////////////////////////////
        /// GPIO & PWM
        ////////////////////////////////////////////////////////////////////////
        void gpioMode(unsigned int pin, unsigned int mode) override;

        void gpioWrite(unsigned int pin, unsigned int state) override;

        unsigned int gpioRead(unsigned int pin) override;

        void gpioSetPwmFrequency(unsigned int pin, unsigned int frequency) override;

        unsigned int gpioGetPwmFrequency(unsigned int pin) override;

        void gpioPwm(unsigned int pin, unsigned int value) override;


        ////////////////////////////////////////////////////////////////////////
        /// I2C
        ////////////////////////////////////////////////////////////////////////
        unsigned int i2cOpen(unsigned int bus, unsigned int address) override;

        void i2cClose(unsigned int handle) override;

        void i2cWriteByte(unsigned int handle, uint8_t data) override;

        uint8_t i2cReadByte(unsigned int handle) override;

        void i2cWriteBytes(unsigned int handle, char *buf, unsigned int count) override;

        unsigned int i2cReadBytes(unsigned int handle, char *buf, unsigned int count) override;

        void i2cWriteReg8(unsigned int handle, uint8_t reg, uint8_t value) override;

        uint8_t i2cReadReg8(unsigned int handle, uint8_t reg) override;

        void i2cWriteReg16(unsigned int handle, uint8_t reg, uint16_t value) override;

        uint16_t i2cReadReg16(unsigned int handle, uint8_t reg) override;


        ////////////////////////////////////////////////////////////////////////
        /// SPI
        ////////////////////////////////////////////////////////////////////////
        
        // Bus = spi bus
        // Channel = which builtin CS pin
        // Mode = SPI Mode (0, 1, 2, 3) 
        unsigned int spiOpen(unsigned int bus, unsigned int channel, unsigned int baud, unsigned int mode) override;

        void spiClose(unsigned int handle) override;

        void spiWrite(unsigned int handle, char *buf, unsigned int count) override;

        unsigned int spiRead(unsigned int handle, char *buf, unsigned int count) override;


        ////////////////////////////////////////////////////////////////////////
        /// UART
        ////////////////////////////////////////////////////////////////////////
        
        unsigned int uartOpen(char *port, unsigned int baud) override;
    
        void uartClose(unsigned int handle) override;

        unsigned int uartAvailable(unsigned int handle) override;

        void uartWrite(unsigned int handle, char* buf, unsigned int count) override;

        unsigned int uartRead(unsigned int handle, char *buf, unsigned int count) override;

        void uartWriteByte(unsigned int handle, uint8_t b) override;

        uint8_t uartReadByte(unsigned int handle) override;
    
    private:
        // Convert pin number to chip number and line number
        // Essentially, just count in order. So first chip's first line is 0
        // Second chip's first line is first chip's last line + 1
        // And so on
        // This is not needed for RPi, where all GPIOs are shown on
        // one gpiochip device, but is needed for other systems
        // eg orangepi 3b (rockchip based) which exposes each bank
        // of 32 lines on its own device.
        std::pair<unsigned int, unsigned int> pinToChipAndLine(unsigned int pin);

        // Convert chip and line to pin number (inverse of above function)
        unsigned int chipAndLineToPin(unsigned int chip, unsigned int line);

        void handleLgpioError(int ec, bool opIsWrite);

        // chip number -> handle
        std::unordered_map<unsigned int, int> chipHandles;

        // chip number -> line count
        std::unordered_map<unsigned int, unsigned int> chipLineCounts;

        // List of GPIO pin numbers which are claimed
        std::unordered_set<unsigned int> claimedGpios;

        // pin -> pwm frequency configured
        std::unordered_map<unsigned int, unsigned int> pwmFreqs;

        // handles we may need to close on termination of provider
        std::unordered_set<int> i2cHandles;
        std::unordered_set<int> spiHandles;
        std::unordered_set<int> uartHandles;

        friend class Io;
    };
}

#endif // HAS_LGPIO
