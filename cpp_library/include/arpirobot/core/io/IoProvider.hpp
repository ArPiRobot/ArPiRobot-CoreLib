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


#include <string>
#include <vector>
#include <mutex>


namespace arpirobot {

    // Forward declare
    class IoDevice;

    // This API closely matches pigpio's API as it is a basic C-style API
    class IoProvider{
    public:
        IoProvider(const IoProvider &other) = delete;
        IoProvider &operator=(const IoProvider &other) = delete;
        virtual ~IoProvider() { }

        ////////////////////////////////////////////////////////////////////////
        /// Constants / Static
        ////////////////////////////////////////////////////////////////////////
        static IoProvider *instance;

        const static unsigned int GPIO_OUT = 0;
        const static unsigned int GPIO_IN = 1;
        const static unsigned int GPIO_LOW = 0;
        const static unsigned int GPIO_HIGH = 1;

        const static char *PROVIDER_PIGPIO;      // PIGPIO library
        const static char *PROVIDER_DUMMY;       // Fake provider. Prints log messages.

        ////////////////////////////////////////////////////////////////////////
        /// Configuration / Control
        ////////////////////////////////////////////////////////////////////////
        static void init(std::string provider = "");

        static void terminate();

        static void addDevice(IoDevice *device);

        static void removeDevice(IoDevice *device);

        ////////////////////////////////////////////////////////////////////////
        /// GPIO & PWM
        ////////////////////////////////////////////////////////////////////////
        virtual void gpioMode(unsigned int pin, unsigned int mode) = 0;

        virtual void gpioWrite(unsigned int pin, unsigned int state) = 0;

        virtual unsigned int gpioRead(unsigned int pin) = 0;

        virtual void gpioSetPwmFrequency(unsigned int pin, unsigned int frequency) = 0;

        virtual unsigned int gpioPwm(unsigned int pin, unsigned int value) = 0;


        ////////////////////////////////////////////////////////////////////////
        /// I2C
        ////////////////////////////////////////////////////////////////////////
        virtual unsigned int i2cOpen(unsigned int bus, unsigned int address) = 0;

        virtual void i2cClose(unsigned int handle) = 0;

        virtual unsigned int i2cWriteByte(unsigned int handle, uint8_t data) = 0;

        virtual uint8_t i2cReadByte(unsigned int handle) = 0;

        virtual uint8_t i2cWriteReg8(unsigned int handle, uint8_t reg, uint8_t value) = 0;

        virtual uint8_t i2cReadReg8(unsigned int handle, uint8_t reg) = 0;

        virtual uint16_t i2cWriteReg16(unsigned int handle, uint8_t reg, uint16_t value) = 0;

        virtual uint16_t i2cReadReg16(unsigned int handle, uint8_t reg) = 0;


        ////////////////////////////////////////////////////////////////////////
        /// SPI
        ////////////////////////////////////////////////////////////////////////
        
        // Bus = spi bus 
        // Channel = which builtin CS pin
        virtual unsigned int spiOpen(unsigned int bus, unsigned int channel) = 0;

        virtual void spiClose(unsigned int handle) = 0;

        virtual void spiWrite(unsigned int handle, uint8_t *buf, unsigned int count) = 0;

        virtual void spiRead(unsigned int handle, uint8_t *buf, unsigned int count) = 0;


        ////////////////////////////////////////////////////////////////////////
        /// UART
        ////////////////////////////////////////////////////////////////////////
        
        virtual unsigned int uartOpen(const char *port, unsigned int buad) = 0;
    
        virtual void uartClose(unsigned int handle) = 0;

        virtual unsigned int uartAvailable(unsigned int handle) = 0;

        virtual void uartWrite(unsigned int handle, const char* buf, unsigned int count) = 0;

        virtual void uartRead(unsigned int handle, const char *buf, unsigned int count) = 0;

    protected:
        IoProvider();

    private:
        static std::vector<IoDevice*> ioDevices;
        static std::mutex ioDevicesLock;
    };

}
