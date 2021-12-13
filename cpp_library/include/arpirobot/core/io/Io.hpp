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
#include <arpirobot/core/io/IoDevice.hpp>
#include <arpirobot/core/io/exceptions.hpp>

namespace arpirobot{

    /**
     * Manages an IoProvider instance and wraps its functions. Also handles IoDevices.
     */
    class Io{
    public:
        ////////////////////////////////////////////////////////////////////////
        /// Constants
        ////////////////////////////////////////////////////////////////////////

        const static unsigned int GPIO_OUT = IoProvider::GPIO_OUT;
        const static unsigned int GPIO_IN = IoProvider::GPIO_IN;
        const static unsigned int GPIO_LOW = IoProvider::GPIO_LOW;
        const static unsigned int GPIO_HIGH = IoProvider::GPIO_HIGH;

        const static char *PROVIDER_PIGPIO;      // PIGPIO library
        const static char *PROVIDER_DUMMY;       // Fake provider. Prints log messages.

        ////////////////////////////////////////////////////////////////////////
        /// IoProvider instance and IoDevice management
        ////////////////////////////////////////////////////////////////////////

        static void init(std::string provider = "");

        static void terminate();

        static void addDevice(IoDevice *device);

        static void removeDevice(IoDevice *device);


        ////////////////////////////////////////////////////////////////////////
        /// Matches pure virtual functions of IoProvider
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        /// GPIO & PWM
        ////////////////////////////////////////////////////////////////////////
        static void gpioMode(unsigned int pin, unsigned int mode);

        static void gpioWrite(unsigned int pin, unsigned int state);

        static unsigned int gpioRead(unsigned int pin);

        static void gpioSetPwmFrequency(unsigned int pin, unsigned int frequency);

        static unsigned int gpioGetPwmFrequency(unsigned int pin);

        static void gpioPwm(unsigned int pin, unsigned int value);


        ////////////////////////////////////////////////////////////////////////
        /// I2C
        ////////////////////////////////////////////////////////////////////////
        static unsigned int i2cOpen(unsigned int bus, unsigned int address);

        static void i2cClose(unsigned int handle);

        static void i2cWriteByte(unsigned int handle, uint8_t data);

        static uint8_t i2cReadByte(unsigned int handle);

        static void i2cWriteBytes(unsigned int handle, char *buf, unsigned int count);

        static unsigned int i2cReadBytes(unsigned int handle, char *buf, unsigned int count);

        static void i2cWriteReg8(unsigned int handle, uint8_t reg, uint8_t value);

        static uint8_t i2cReadReg8(unsigned int handle, uint8_t reg);

        static void i2cWriteReg16(unsigned int handle, uint8_t reg, uint16_t value);

        static uint16_t i2cReadReg16(unsigned int handle, uint8_t reg);


        ////////////////////////////////////////////////////////////////////////
        /// SPI
        ////////////////////////////////////////////////////////////////////////
        
        // Bus = spi bus 
        // Channel = which builtin CS pin
        static unsigned int spiOpen(unsigned int bus, unsigned int channel, unsigned int baud);

        static void spiClose(unsigned int handle);

        static void spiWrite(unsigned int handle, char *buf, unsigned int count);

        static unsigned int spiRead(unsigned int handle, char *buf, unsigned int count);


        ////////////////////////////////////////////////////////////////////////
        /// UART
        ////////////////////////////////////////////////////////////////////////
        
        static unsigned int uartOpen(char *port, unsigned int baud);
    
        static void uartClose(unsigned int handle);

        static unsigned int uartAvailable(unsigned int handle);

        static void uartWrite(unsigned int handle, char* buf, unsigned int count);

        static unsigned int uartRead(unsigned int handle, char *buf, unsigned int count);

        static void uartWriteByte(unsigned int handle, uint8_t b);

        static uint8_t uartReadByte(unsigned int handle);

    private:
        static IoProvider *instance;
        static std::vector<IoDevice*> ioDevices;
        static std::mutex ioDevicesLock;
    };

}