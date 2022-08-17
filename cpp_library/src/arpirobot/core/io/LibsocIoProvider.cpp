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
#include <arpirobot/core/io/exceptions.hpp>
#include <chrono>
#include <functional>

using namespace arpirobot;


unsigned int LibsocIoProvider::nextI2cHandle = 0;
unsigned int LibsocIoProvider::nextSpiHandle = 0;


LibsocIoProvider::LibsocIoProvider(){
#ifdef HAS_SERIAL
    Logger::logInfoFrom("LibsocIoProvider", "Wrapping SerialIoProvider for UART support.");
    uartProvider = new SerialIoProvider(true);
#else
    Logger::logWarning("Using libsoc without serial. UART will not function.");
#endif
    // No actual initialization to be done for libsoc itself
    Logger::logInfoFrom("LibsocIoProvider", "IO Provider initialized.");

}

LibsocIoProvider::~LibsocIoProvider(){
#ifdef HAS_SERIAL
    if(uartProvider != nullptr){
        delete uartProvider;
    }
#endif
    
    // Stop all pwm threads & free resources
    for(auto &it : pwmMap){
        if(it.second->alive){
            it.second->alive = false;
            it.second->thread->join();
        }
        delete it.second;
    }
    pwmMap.clear();

    // Free all gpios
    for(auto &it : gpioMap){
        libsoc_gpio_free(it.second);
    }
    gpioMap.clear();

    // Free any active I2C
    for(auto &it : i2cMap){
        libsoc_i2c_free(it.second);
    }
    i2cMap.clear();

    // Free any active SPI
    for(auto &it : spiMap){
        libsoc_spi_free(it.second);
    }
    spiMap.clear();

    Logger::logInfoFrom("LibsocIoProvider", "IO Provider terminated.");
}

////////////////////////////////////////////////////////////////////////
/// GPIO & PWM
////////////////////////////////////////////////////////////////////////
void LibsocIoProvider::gpioMode(unsigned int pin, unsigned int mode){
    // Disable software PWM for this pin if it was previously in use
    auto it = pwmMap.find(pin);
    if(it != pwmMap.end()){
        if(it->second->alive){
            it->second->alive = false;
            it->second->thread->join();
            delete it->second->thread;
            it->second->thread = nullptr;
        }
    }

    int res = libsoc_gpio_set_direction(getGpio(pin), toLibsocMode(mode));
    if(res == EXIT_FAILURE){
        throw AccessException();
    }
}

void LibsocIoProvider::gpioWrite(unsigned int pin, unsigned int state){
    // Disable software PWM for this pin if it was previously in use
    auto it = pwmMap.find(pin);
    if(it != pwmMap.end()){
        if(it->second->alive){
            it->second->alive = false;
            it->second->thread->join();
            delete it->second->thread;
            it->second->thread = nullptr;
        }
    }

    int res = libsoc_gpio_set_level(getGpio(pin), toLibsocState(state));
    if (res == EXIT_FAILURE){
        throw AccessException();
    }
}

unsigned int LibsocIoProvider::gpioRead(unsigned int pin){
    gpio_level res = libsoc_gpio_get_level(getGpio(pin));
    if(res == LEVEL_ERROR){
        throw AccessException();
    }
    return fromLibsocState(res);
}

void LibsocIoProvider::gpioSetPwmFrequency(unsigned int pin, unsigned int frequency){
    // Write new frequency
    pwmconfig *pwm = getPwm(pin);
    pwm->frequency = frequency;

    // Recalculate on and off times
    unsigned long totalTime = (unsigned long)((1.0 / pwm->frequency) * 1e9);    // in ns
    pwm->onTime = (pwm->dutyCycle / 255.0) * totalTime;
    pwm->offTime = totalTime - pwm->onTime;
}

unsigned int LibsocIoProvider::gpioGetPwmFrequency(unsigned int pin){
    return getPwm(pin)->frequency;    
}

void LibsocIoProvider::gpioPwm(unsigned int pin, unsigned int value){
    // Write new duty cycle
    pwmconfig *pwm = getPwm(pin);
    if(value > 255)
        value = 255;
    pwm->dutyCycle = value;

    // Recalculate on and off times
    unsigned long totalTime = (unsigned long)((1.0 / pwm->frequency) * 1e9);    // in ns
    pwm->onTime = (pwm->dutyCycle / 255.0) * totalTime;
    pwm->offTime = totalTime - pwm->onTime;

    // Start pwm thread for this pin if not already running
    if(!pwm->alive){
        pwm->alive = true;
        pwm->thread = new std::thread(std::bind(&LibsocIoProvider::pwmThread, this, pwm));
    } 
}


////////////////////////////////////////////////////////////////////////
/// I2C
////////////////////////////////////////////////////////////////////////
unsigned int LibsocIoProvider::i2cOpen(unsigned int bus, unsigned int address){
    i2c *i = libsoc_i2c_init(bus, address);
    if(i == NULL){
        throw InvalidParameterException();
    }
    i2cMap[nextI2cHandle] = i;
    nextI2cHandle++;
    return nextI2cHandle - 1;
}

void LibsocIoProvider::i2cClose(unsigned int handle){
    auto it = i2cMap.find(handle);
    if(it == i2cMap.end())
        throw BadHandleException();
    libsoc_i2c_free(it->second);
    i2cMap.erase(handle);
}

void LibsocIoProvider::i2cWriteByte(unsigned int handle, uint8_t data){
    auto it = i2cMap.find(handle);
    if(it == i2cMap.end())
        throw BadHandleException();
    if(libsoc_i2c_write(it->second, &data, 1) == EXIT_FAILURE){
        throw WriteFailedException();
    }
}

uint8_t LibsocIoProvider::i2cReadByte(unsigned int handle){
    auto it = i2cMap.find(handle);
    if(it == i2cMap.end())
        throw BadHandleException();
    uint8_t buf;
    if(libsoc_i2c_read(it->second, &buf, 1) == EXIT_FAILURE)
        throw ReadFailedException();
    return buf;
}

void LibsocIoProvider::i2cWriteBytes(unsigned int handle, char *buf, unsigned int count){
    auto it = i2cMap.find(handle);
    if(it == i2cMap.end())
        throw BadHandleException();
    uint8_t *byteBuf = new uint8_t[count];
    for(size_t i = 0; i < count; ++i){
        byteBuf[i] = buf[i];
    }
    if(libsoc_i2c_write(it->second, byteBuf, count) == EXIT_FAILURE){
        delete[] byteBuf;
        throw WriteFailedException();
    }
    delete[] byteBuf;
}

unsigned int LibsocIoProvider::i2cReadBytes(unsigned int handle, char *buf, unsigned int count){
    auto it = i2cMap.find(handle);
    if(it == i2cMap.end())
        throw BadHandleException();
    uint8_t *byteBuf = new uint8_t[count];
    if(libsoc_i2c_read(it->second, byteBuf, count) == EXIT_FAILURE){
        delete[] byteBuf;
        throw ReadFailedException();
    }
    for(size_t i = 0; i < count; ++i){
        buf[i] = byteBuf[i];
    }
    delete[] byteBuf;
    return count;
}

void LibsocIoProvider::i2cWriteReg8(unsigned int handle, uint8_t reg, uint8_t value){
    auto it = i2cMap.find(handle);
    if(it == i2cMap.end())
        throw BadHandleException();
    uint8_t buf[2];
    buf[0] = reg;
    buf[1] = value;
    if(libsoc_i2c_write(it->second, buf, 2) == EXIT_FAILURE)
        throw WriteFailedException();
}

uint8_t LibsocIoProvider::i2cReadReg8(unsigned int handle, uint8_t reg){
    auto it = i2cMap.find(handle);
    if(it == i2cMap.end())
        throw BadHandleException();
    uint8_t buf;
    if(libsoc_i2c_write(it->second, &reg, 1) == EXIT_FAILURE)
        throw WriteFailedException();
    if(libsoc_i2c_read(it->second, &buf, 1) == EXIT_FAILURE)
        throw ReadFailedException();
}

void LibsocIoProvider::i2cWriteReg16(unsigned int handle, uint8_t reg, uint16_t value){
    // Byte order determined by I2C device, thus this can't really be implemented sw level
    throw NotImplementedByProviderException();
}

uint16_t LibsocIoProvider::i2cReadReg16(unsigned int handle, uint8_t reg){
    // Byte order determined by I2C device, thus this can't really be implemented sw level
    throw NotImplementedByProviderException();
}


////////////////////////////////////////////////////////////////////////
/// SPI
////////////////////////////////////////////////////////////////////////

unsigned int LibsocIoProvider::spiOpen(unsigned int bus, unsigned int channel, unsigned int baud, unsigned int mode){
    spi *s = libsoc_spi_init(bus, channel);
    if(s == NULL){
        throw InvalidParameterException();
    }
    spi_mode m = spi_mode::MODE_ERROR;
    switch(mode){
    case 0:
        m = spi_mode::MODE_0;
        break;
    case 1:
        m = spi_mode::MODE_1;
        break;
    case 2:
        m = spi_mode::MODE_2;
        break;
    case 3:
        m = spi_mode::MODE_3;
        break;
    }
    if(libsoc_spi_set_speed(s, baud) == EXIT_FAILURE)
        throw InvalidParameterException();
    if(libsoc_spi_set_mode(s, m) == EXIT_FAILURE)
        throw InvalidParameterException();
    spiMap[nextSpiHandle] = s;
    nextSpiHandle++;
    return nextSpiHandle - 1;
}

void LibsocIoProvider::spiClose(unsigned int handle){
    auto it = spiMap.find(handle);
    if(it == spiMap.end())
        throw BadHandleException();
    libsoc_spi_free(it->second);
    spiMap.erase(handle);
}

void LibsocIoProvider::spiWrite(unsigned int handle, char *buf, unsigned int count){
    auto it = spiMap.find(handle);
    if(it == spiMap.end())
        throw BadHandleException();
    uint8_t *byteBuf = new uint8_t[count];
    for(size_t i = 0; i < count; ++i){
        byteBuf[i] = buf[i];
    }
    if(libsoc_spi_write(it->second, byteBuf, count) == EXIT_FAILURE){
        delete[] byteBuf;
        throw WriteFailedException();
    }
    delete[] byteBuf;
}

unsigned int LibsocIoProvider::spiRead(unsigned int handle, char *buf, unsigned int count){
    auto it = spiMap.find(handle);
    if(it == spiMap.end())
        throw BadHandleException();
    uint8_t *byteBuf = new uint8_t[count];
    if(libsoc_spi_read(it->second, byteBuf, count) == EXIT_FAILURE){
        delete[] byteBuf;
        throw ReadFailedException();
    }
    for(size_t i = 0; i < count; ++i){
        buf[i] = byteBuf[i];
    }
    delete[] byteBuf;
    return count;
}


////////////////////////////////////////////////////////////////////////
/// UART
////////////////////////////////////////////////////////////////////////

unsigned int LibsocIoProvider::uartOpen(char *port, unsigned int baud){
#ifdef HAS_SERIAL
    return uartProvider->uartOpen(port, baud);
#else
    throw NotImplementedByProviderException();
#endif
}

void LibsocIoProvider::uartClose(unsigned int handle){
#ifdef HAS_SERIAL
    uartProvider->uartClose(handle);
#else

#endif
}

unsigned int LibsocIoProvider::uartAvailable(unsigned int handle){
#ifdef HAS_SERIAL
    return uartProvider->uartAvailable(handle);
#else
    throw NotImplementedByProviderException();
#endif
}

void LibsocIoProvider::uartWrite(unsigned int handle, char* buf, unsigned int count){
#ifdef HAS_SERIAL
    return uartProvider->uartWrite(handle, buf, count);
#else
    throw NotImplementedByProviderException();
#endif
}

unsigned int LibsocIoProvider::uartRead(unsigned int handle, char *buf, unsigned int count){
#ifdef HAS_SERIAL
    return uartProvider->uartRead(handle, buf, count);
#else
    throw NotImplementedByProviderException();
#endif
}

void LibsocIoProvider::uartWriteByte(unsigned int handle, uint8_t b){
#ifdef HAS_SERIAL
    uartProvider->uartWriteByte(handle, b);
#else
    throw NotImplementedByProviderException();
#endif
}

uint8_t LibsocIoProvider::uartReadByte(unsigned int handle){
#ifdef HAS_SERIAL
    return uartProvider->uartReadByte(handle);
#else
    throw NotImplementedByProviderException();
#endif
}



////////////////////////////////////////////////////////////////////////////////
/// libsoc helper functions
////////////////////////////////////////////////////////////////////////////////

void LibsocIoProvider::pwmThread(pwmconfig *pwm){
    libsoc_gpio_set_direction(pwm->g, gpio_direction::OUTPUT);
    while(pwm->alive){
        if(pwm->onTime > 0){
            libsoc_gpio_set_level(pwm->g, gpio_level::HIGH);
            std::this_thread::sleep_for(std::chrono::nanoseconds(pwm->onTime));
        }
        if(pwm->offTime > 0){
            libsoc_gpio_set_level(pwm->g, gpio_level::LOW);
            std::this_thread::sleep_for(std::chrono::nanoseconds(pwm->offTime));
        }
    }
    libsoc_gpio_set_level(pwm->g, gpio_level::LOW);
}

gpio *LibsocIoProvider::getGpio(unsigned int pin){
    auto it = gpioMap.find(pin);
    if(it == gpioMap.end()){
        gpioMap[pin] = libsoc_gpio_request(pin, gpio_mode::LS_GPIO_SHARED);
        if(gpioMap[pin] == NULL){
            throw InvalidPinException();
        }
        return gpioMap[pin];
    }else{
        return it->second;
    }
}

gpio_direction LibsocIoProvider::toLibsocMode(int mode){
    if(mode == GPIO_OUT) return gpio_direction::OUTPUT;
    if(mode == GPIO_IN) return gpio_direction::INPUT;
    throw InvalidParameterException();
}
        
int LibsocIoProvider::fromLibsocMode(gpio_direction mode){
    if(mode == gpio_direction::INPUT) return GPIO_IN;
    if(mode == gpio_direction::OUTPUT) return GPIO_OUT;
    throw InvalidParameterException();
}

gpio_level LibsocIoProvider::toLibsocState(int state){
    if(state == GPIO_HIGH) return gpio_level::HIGH;
    if(state == GPIO_LOW) return gpio_level::LOW;
    throw InvalidParameterException();
}

int LibsocIoProvider::fromLibsocState(gpio_level state){
    if(state == gpio_level::HIGH) return GPIO_HIGH;
    if(state == gpio_level::LOW) return GPIO_LOW;
    throw InvalidParameterException();
}

LibsocIoProvider::pwmconfig *LibsocIoProvider::getPwm(unsigned int pin){
    auto it = pwmMap.find(pin);
    if(it == pwmMap.end()){
        pwmMap[pin] = new pwmconfig();
        pwmMap[pin]->alive = false;
        pwmMap[pin]->dutyCycle = 0;
        pwmMap[pin]->frequency = 100;
        pwmMap[pin]->g = getGpio(pin);
        pwmMap[pin]->offTime = 10000000;
        pwmMap[pin]->onTime = 0;
        pwmMap[pin]->thread = nullptr;
        return pwmMap[pin];
    }else{
        return it->second;
    }
}

#endif // HAS_LIBSOC