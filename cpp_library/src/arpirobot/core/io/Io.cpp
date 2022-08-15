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


#include <arpirobot/core/io/Io.hpp>
#include <arpirobot/core/io/DummyIoProvider.hpp>
#include <arpirobot/core/io/PigpioIoProvider.hpp>
#include <arpirobot/core/io/SerialIoProvider.hpp>

#include <algorithm>
#include <fstream>
#include <cstdio>

using namespace arpirobot;


IoProvider *Io::instance = nullptr;
std::vector<IoDevice*> Io::ioDevices;
std::mutex Io::ioDevicesLock;

const char *Io::PROVIDER_PIGPIO = "pigpio";
const char *Io::PROVIDER_DUMMY = "dummy";
const char *Io::PROVIDER_SERIAL = "serial";



void Io::init(std::string provider){
    // Choose a default provider based on the current platform if none is specified

    if(provider == ""){
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    // Windows OS
    provider = "serial";
#elif defined(__APPLE__)
    // macOS
    provider = "serial";
#elif defined(__linux__) or defined(linux) or defined(__linux)
    // Linux

    // Determine if this is a raspberry pi board
    bool isrpi = false;
    FILE *pipe = popen("cat /proc/cpuinfo | grep Model", "r");
    if(pipe){
        char buf[64];
        std::string cmdOutput;
        while(!feof(pipe)){
            if(fgets(buf, 64, pipe) != NULL)
                cmdOutput += buf;
        }
        pclose(pipe);
        isrpi = (cmdOutput.find("Raspberry Pi") != std::string::npos);
    }

    if(isrpi){
        // Raspberry pi
        provider = "pigpio";
    }else{
        // Not a raspberry pi
        // TODO: libsoc
        provider = "serial";
    }
#else
    // Unknown OS
    provider = "dummy";
#endif
    }

    // Terminate old provider first (if one)
    terminate();

    // Instantiate the requested provider. These can throw exceptions if a fatal error occurs
    if(provider == PROVIDER_PIGPIO){
#ifdef HAS_PIGPIO
        instance = new PigpioIoProvider();
#else
        throw std::runtime_error("The IO provider '" + provider + "' is not supported on this platform.");
#endif
    }else if(provider == PROVIDER_SERIAL){
#ifdef HAS_SERIAL
        instance = new SerialIoProvider();
#else
        throw std::runtime_error("The IO provider '" + provider + "' is not supported on this platform.");
#endif
    // TODO: libsoc once implemented
    }else if(provider == PROVIDER_DUMMY){
        instance = new DummyIoProvider();
    }else{
        throw std::runtime_error("The IO provider '" + provider + "' is unknown.");
    }
}

void Io::terminate(){
    if(instance != nullptr){
        {
            std::lock_guard<std::mutex> l(ioDevicesLock);
            for(IoDevice *dev : ioDevices){
                try{
                    dev->close();
                }catch(std::exception &e){
                    // Silently fail closing device. Device was likely not opened properly anyways
                }
            }
            ioDevices.clear();
        }

        delete instance;
        instance = nullptr;
    }
}

void Io::addDevice(IoDevice *device){
    std::lock_guard<std::mutex> l(ioDevicesLock);
    if(std::find(ioDevices.begin(), ioDevices.end(), device) == ioDevices.end()){
        ioDevices.push_back(device);
    }
}

void Io::removeDevice(IoDevice *device){
    std::lock_guard<std::mutex> l(ioDevicesLock);
    auto it = std::find(ioDevices.begin(), ioDevices.end(), device);
    if(it != ioDevices.end()){
        ioDevices.erase(it);
    }
}


void Io::gpioMode(unsigned int pin, unsigned int mode){
    if(instance != nullptr){
        instance->gpioMode(pin, mode);
    }
}

void Io::gpioWrite(unsigned int pin, unsigned int state){
    if(instance != nullptr){
        instance->gpioWrite(pin, state);
    }
}

unsigned int Io::gpioRead(unsigned int pin){
    if(instance != nullptr){
        return instance->gpioRead(pin);
    }
    return 0;
}

void Io::gpioSetPwmFrequency(unsigned int pin, unsigned int frequency){
    if(instance != nullptr){
        instance->gpioSetPwmFrequency(pin, frequency);
    }
}

unsigned int Io::gpioGetPwmFrequency(unsigned int pin){
    if(instance != nullptr){
        return instance->gpioGetPwmFrequency(pin);
    }
    return 0;
}


void Io::gpioPwm(unsigned int pin, unsigned int value){
    if(instance != nullptr){
        instance->gpioPwm(pin, value);
    }
}


unsigned int Io::i2cOpen(unsigned int bus, unsigned int address){
    if(instance != nullptr){
        return instance->i2cOpen(bus, address);
    }
    return -1; // -1 interpreted as max unsigned int value
}

void Io::i2cClose(unsigned int handle){
    if(instance != nullptr){
        instance->i2cClose(handle);
    }
}

void Io::i2cWriteByte(unsigned int handle, uint8_t data){
    if(instance != nullptr){
        instance->i2cWriteByte(handle, data);
    }
}

uint8_t Io::i2cReadByte(unsigned int handle){
    if(instance != nullptr){
        return instance->i2cReadByte(handle);
    }
    return 0;
}

void Io::i2cWriteBytes(unsigned int handle, char *buf, unsigned int count){
    if(instance != nullptr){
        instance->i2cWriteBytes(handle, buf, count);
    }
}

unsigned int Io::i2cReadBytes(unsigned int handle, char *buf, unsigned int count){
    if(instance != nullptr){
        return instance->i2cReadBytes(handle, buf, count);
    }
    return 0;
}

void Io::i2cWriteReg8(unsigned int handle, uint8_t reg, uint8_t value){
    if(instance != nullptr){
        instance->i2cWriteReg8(handle, reg, value);
    }
}

uint8_t Io::i2cReadReg8(unsigned int handle, uint8_t reg){
    if(instance != nullptr){
        return instance->i2cReadReg8(handle, reg);
    }
    return 0;
}

void Io::i2cWriteReg16(unsigned int handle, uint8_t reg, uint16_t value){
    if(instance != nullptr){
        instance->i2cWriteReg16(handle, reg, value);
    }
}

uint16_t Io::i2cReadReg16(unsigned int handle, uint8_t reg){
    if(instance != nullptr){
        return instance->i2cReadReg16(handle, reg);
    }
    return 0;
}


unsigned int Io::spiOpen(unsigned int bus, unsigned int channel, unsigned int baud, unsigned int mode){
    if(instance != nullptr){
        return instance->spiOpen(bus, channel, baud, mode);
    }
    return -1; // -1 interpreted as max unsigned int
}

void Io::spiClose(unsigned int handle){
    if(instance != nullptr){
        instance->spiClose(handle);
    }
}

void Io::spiWrite(unsigned int handle, char *buf, unsigned int count){
    if(instance != nullptr){
        instance->spiWrite(handle, buf, count);
    }
}

unsigned int Io::spiRead(unsigned int handle, char *buf, unsigned int count){
    if(instance != nullptr){
        return instance->spiRead(handle, buf, count);
    }
    return 0;
}


unsigned int Io::uartOpen(char *port, unsigned int baud){
    if(instance != nullptr){
        return instance->uartOpen(port, baud);
    }
    return -1; // -1 interpreted as max unsigned int
}

void Io::uartClose(unsigned int handle){
    if(instance != nullptr){
        instance->uartClose(handle);
    }
}

unsigned int Io::uartAvailable(unsigned int handle){
    if(instance != nullptr){
        return instance->uartAvailable(handle);
    }
    return 0;
}

void Io::uartWrite(unsigned int handle, char* buf, unsigned int count){
    if(instance != nullptr){
        instance->uartWrite(handle, buf, count);
    }
}

unsigned int Io::uartRead(unsigned int handle, char *buf, unsigned int count){
    if(instance != nullptr){
        return instance->uartRead(handle, buf, count);
    }
    return 0;
}

void Io::uartWriteByte(unsigned int handle, uint8_t b){
    if(instance != nullptr){
        instance->uartWriteByte(handle, b);
    }
}

uint8_t Io::uartReadByte(unsigned int handle){
    if(instance != nullptr){
        return instance->uartReadByte(handle);
    }
    return 0;
}
