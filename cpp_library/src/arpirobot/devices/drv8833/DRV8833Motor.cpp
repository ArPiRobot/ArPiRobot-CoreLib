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

#include <arpirobot/devices/drv8833/DRV8833Motor.hpp>
#include <arpirobot/core/robot/BaseRobot.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/io/Io.hpp>

using namespace arpirobot;

DRV8833Motor::DRV8833Motor(int in1Pin, int in2Pin, int slpPin) : 
        IoDevice(), in1(in1Pin), in2(in2Pin), slp(slpPin){
    deviceName = "DRV8833Motor(" + std::to_string(in1) + ", " + std::to_string(in2) + ", " + std::to_string(slp) + ")";
    // Always call this at the end of the device's constructor
    BaseRobot::beginWhenReady(this);
}

DRV8833Motor::~DRV8833Motor(){
    close();
}

void DRV8833Motor::begin(){
    try{
        Io::gpioMode(in1, Io::GPIO_OUT);
        Io::gpioMode(in2, Io::GPIO_OUT);
        Io::gpioMode(slp, Io::GPIO_OUT);
        Io::gpioPwm(in1, 0);
        Io::gpioPwm(in2, 0);
        Io::gpioWrite(slp, Io::GPIO_HIGH);
    }catch(const std::exception &e){
        Logger::logErrorFrom(getDeviceName(), "Failed to initialize device. GPIO error.");
        Logger::logDebugFrom(getDeviceName(), e.what());
    }
}

void DRV8833Motor::run(){
    try{
        if(speed == 0){
            if(brakeMode){
                Io::gpioPwm(in1, 255);
                Io::gpioPwm(in2, 255);
            }else{
                Io::gpioPwm(in1, 0);
                Io::gpioPwm(in2, 0);
            }
        }else{
            int spd = (int)(std::abs(speed) * 255);
            if(speed > 0){
                Io::gpioPwm(in1, spd);
                Io::gpioPwm(in2, 0);
            }else{
                Io::gpioPwm(in1, 0);
                Io::gpioPwm(in2, spd);
            }
        }
    }catch(const std::exception &e){
        Logger::logWarningFrom(getDeviceName(), "Failed to set motor speed.");
        Logger::logDebugFrom(getDeviceName(), e.what());
    }
}

void DRV8833Motor::close(){
    try{
        Io::gpioPwm(in1, 0);
        Io::gpioPwm(in2, 0);
        Io::gpioWrite(slp, 0);
    }catch(const std::exception &e){
        // Silently fail
    }
}
