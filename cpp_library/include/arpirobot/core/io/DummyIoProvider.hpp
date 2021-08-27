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

#pragma once

#include <arpirobot/core/io/IoProvider.hpp>

namespace arpirobot{
    class DummyIoProvider : public IoProvider{
    public:

        DummyIoProvider();
        
        ~DummyIoProvider();

        ////////////////////////////////////////////////////////////////////////
        /// GPIO & PWM
        ////////////////////////////////////////////////////////////////////////
        void gpioMode(unsigned int pin, unsigned int mode);

        void gpioWrite(unsigned int pin, unsigned int state);

        unsigned int gpioRead(unsigned int pin);

        void gpioSetPwmFrequency(unsigned int pin, unsigned int frequency);

        unsigned int gpioPwm(unsigned int pin, unsigned int value);


        ////////////////////////////////////////////////////////////////////////
        /// I2C
        ////////////////////////////////////////////////////////////////////////
        unsigned int i2cOpen(unsigned int bus, unsigned int address);

        void i2cClose(unsigned int handle);

        unsigned int i2cWriteByte(unsigned int handle, uint8_t data);

        uint8_t i2cReadByte(unsigned int handle);

        uint8_t i2cWriteReg8(unsigned int handle, uint8_t reg, uint8_t value);

        uint8_t i2cReadReg8(unsigned int handle, uint8_t reg);

        uint16_t i2cWriteReg16(unsigned int handle, uint8_t reg, uint16_t value);

        uint16_t i2cReadReg16(unsigned int handle, uint8_t reg);


        ////////////////////////////////////////////////////////////////////////
        /// SPI
        ////////////////////////////////////////////////////////////////////////
        
        // Bus = spi bus 
        // Channel = which builtin CS pin
        unsigned int spiOpen(unsigned int bus, unsigned int channel);

        void spiClose(unsigned int handle);

        void spiWrite(unsigned int handle, uint8_t *buf, unsigned int count);

        void spiRead(unsigned int handle, uint8_t *buf, unsigned int count);


        ////////////////////////////////////////////////////////////////////////
        /// UART
        ////////////////////////////////////////////////////////////////////////
        
        unsigned int uartOpen(const char *port, unsigned int buad);
    
        void uartClose(unsigned int handle);

        unsigned int uartAvailable(unsigned int handle);

        void uartWrite(unsigned int handle, const char* buf, unsigned int count);

        void uartRead(unsigned int handle, const char *buf, unsigned int count);
    };
}
