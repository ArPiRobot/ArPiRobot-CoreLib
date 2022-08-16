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


// TODO: Maybe implement hardware PWM use with software fallback
// This may require device-specific knowledge (ie what gpio number maps to which pwm channel)

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
    if(uartProvider != nullptr){
        delete uartProvider;
    }
    
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

    // TODO: Close I2C, PWM, SPI, etc

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