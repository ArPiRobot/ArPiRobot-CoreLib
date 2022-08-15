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

#ifdef HAS_PIGPIO

#include <arpirobot/core/io/IoProvider.hpp>
#include <pigpio.h>

namespace arpirobot{
    class PigpioIoProvider : public IoProvider{
    protected:

        PigpioIoProvider();

        ~PigpioIoProvider();

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
        void handlePigpioError(int ec, bool opIsWrite);
        
        int toPigpioMode(int mode);
        
        int fromPigpioMode(int mode);

        int toPigpioState(int state);

        int fromPigpioState(int state);

        friend class Io;
    };
}

#endif // HAS_PIGPIO
