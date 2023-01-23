
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

#include <arpirobot/devices/gpio/GPIOPin.hpp>
#include <arpirobot/core/io/Io.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/robot/BaseRobot.hpp>

using namespace arpirobot;

////////////////////////////////////////////////////////////////////////////////
/// GPIOPin
////////////////////////////////////////////////////////////////////////////////

GPIOPin::GPIOPin(unsigned int pin) : pin(pin), lastPwmValue(0){
    deviceName = "GPIOPin(" + std::to_string(pin) + ")";
    BaseRobot::beginWhenReady(this);
}

GPIOPin::~GPIOPin(){
    close();
}

// Pin configuration
void GPIOPin::setMode(Mode mode){
    if(mode == Mode::Input){
        Io::gpioMode(pin, Io::GPIO_IN);
    }else{
        Io::gpioMode(pin, Io::GPIO_OUT);
    }
}

// Digital logic
void GPIOPin::setLevel(Level level){
    if(level == Level::High){
        Io::gpioWrite(pin, Io::GPIO_HIGH);
    }else{
        Io::gpioWrite(pin, Io::GPIO_LOW);
    }
}

GPIOPin::Level GPIOPin::getLevel(){
    unsigned int level = Io::gpioRead(pin);
    if(level == Io::GPIO_HIGH) return Level::High;
    return Level::Low;
}

// PWM
void GPIOPin::setPwmValue(uint8_t val){
    lastPwmValue = val;
    Io::gpioPwm(pin, val);
}

uint8_t GPIOPin::getPwmValue(){
    return lastPwmValue;
}

void GPIOPin::setPwmFrequency(unsigned int freq){
    Io::gpioSetPwmFrequency(pin, freq);
}

unsigned int GPIOPin::getPwmFrequency(){
    return Io::gpioGetPwmFrequency(pin);
}

void GPIOPin::close(){
    Io::gpioWrite(pin, Io::GPIO_LOW);
}

void GPIOPin::begin(){
    // Nothing to do here
}

bool GPIOPin::isEnabled(){
    return true;
}

bool GPIOPin::shouldMatchRobotState(){
    return false;
}

bool GPIOPin::shouldDisableWithWatchdog(){
    return false;
}

void GPIOPin::enable(){

}

void GPIOPin::disable(){
    
}
