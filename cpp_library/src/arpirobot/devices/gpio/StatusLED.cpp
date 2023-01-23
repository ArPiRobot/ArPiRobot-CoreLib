
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

#include <arpirobot/devices/gpio/StatusLED.hpp>
#include <arpirobot/core/io/Io.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/robot/BaseRobot.hpp>

using namespace arpirobot;

////////////////////////////////////////////////////////////////////////////////
/// StatusLED
////////////////////////////////////////////////////////////////////////////////

StatusLED::StatusLED(unsigned int pin) : pin(pin){
    deviceName = "StatusLED(" + std::to_string(pin) + ")";
    BaseRobot::beginWhenReady(this);
}

StatusLED::~StatusLED(){
    if(schedulerTask != nullptr){
        BaseRobot::removeTaskFromScheduler(schedulerTask);
    }
    close();
}

void StatusLED::close(){
    Io::gpioWrite(pin, Io::GPIO_LOW);
}

void StatusLED::begin(){
    try{
        Io::gpioMode(pin, Io::GPIO_OUT);
    }catch(const std::exception &e){
        Logger::logErrorFrom(getDeviceName(), "Failed to initialize device. GPIO error.");
        Logger::logDebugFrom(getDeviceName(), e.what());
    }
}

bool StatusLED::isEnabled(){
    return schedulerTask != nullptr;
}

bool StatusLED::shouldMatchRobotState(){
    return true;
}

bool StatusLED::shouldDisableWithWatchdog(){
    return false;
}

void StatusLED::enable(){
    schedulerTask = BaseRobot::scheduleRepeatedFunction(
        std::bind(&StatusLED::blink, this), std::chrono::milliseconds(500));
}

void StatusLED::disable(){
    if(schedulerTask != nullptr)
        BaseRobot::removeTaskFromScheduler(schedulerTask);
    schedulerTask = nullptr;
    Io::gpioWrite(pin, Io::GPIO_HIGH);
}

void StatusLED::blink(){
    if(!isEnabled())
        return;
    if(Io::gpioRead(pin) == Io::GPIO_HIGH){
        Io::gpioWrite(pin, Io::GPIO_LOW);
    }else{
        Io::gpioWrite(pin, Io::GPIO_HIGH);
    }
}
