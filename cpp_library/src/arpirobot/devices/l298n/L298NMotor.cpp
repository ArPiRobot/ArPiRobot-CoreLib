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

#include <arpirobot/devices/l298n/L298NMotor.hpp>
#include <arpirobot/core/robot/BaseRobot.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/io/Io.hpp>


using namespace arpirobot;

L298NMotor::L298NMotor(int in1Pin, int in2Pin, int pwmPin) : in1(in1Pin), in2(in2Pin), pwm(pwmPin){
    // Always call this at the end of the device's constructor
    BaseRobot::beginWhenReady(this);
}

L298NMotor::~L298NMotor(){
    Io::gpioWrite(in1, 0);
    Io::gpioWrite(in2, 0);
    Io::gpioPwm(pwm, 0);
}

std::string L298NMotor::getDeviceName(){
    return "L298NMotor(" + std::to_string(in1) + ", " + std::to_string(in2) + ", " + std::to_string(pwm) + ")";
}

void L298NMotor::begin(){
    Io::gpioMode(in1, Io::GPIO_OUT);
    Io::gpioMode(in2, Io::GPIO_OUT);
    Io::gpioPwm(pwm, 0);
}

void L298NMotor::run(){
    if(speed == 0){
        Io::gpioWrite(in1, 0);
        Io::gpioWrite(in2, 0);
        if(brakeMode){
            Io::gpioPwm(pwm, 255);
        }else{
            Io::gpioPwm(pwm, 0);
        }
    }else{
        if(speed > 0){
            Io::gpioWrite(in1, 1);
            Io::gpioWrite(in2, 0);
        }else{
            Io::gpioWrite(in1, 0);
            Io::gpioWrite(in2, 1);
        }
        Io::gpioPwm(pwm, (int)(std::abs(speed) * 255));
    }
}