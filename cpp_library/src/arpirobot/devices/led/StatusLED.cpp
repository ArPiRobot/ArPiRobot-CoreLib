
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

#include <arpirobot/devices/led/StatusLED.hpp>
#include <arpirobot/core/io/Io.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/robot/BaseRobot.hpp>

using namespace arpirobot;

////////////////////////////////////////////////////////////////////////////////
/// StatusLED
////////////////////////////////////////////////////////////////////////////////

StatusLED::StatusLED(int pin) : pin(pin){
    BaseRobot::beginWhenReady(this);
}

StatusLED::~StatusLED(){
    stop = true;
    close();
}

std::string StatusLED::getDeviceName(){
    return "StatusLED(" + std::to_string(pin) + ")";
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
    BaseRobot::runOnceSoon(std::bind(&StatusLED::feed, this));
}

bool StatusLED::isEnabled(){
    return enabled;
}

bool StatusLED::shouldMatchRobotState(){
    return true;
}

bool StatusLED::shouldDisableWithWatchdog(){
    return false;
}

void StatusLED::enable(){
    enabled = true;
}

void StatusLED::disable(){
    enabled = false;
    Io::gpioWrite(pin, Io::GPIO_HIGH);
}

void StatusLED::feed(){
    while(!stop && BaseRobot::currentRobot != nullptr){
        if(enabled){
            try{
                if(ledState){
                    Io::gpioWrite(pin, Io::GPIO_HIGH);
                }else{
                    Io::gpioWrite(pin, Io::GPIO_LOW);
                }
                ledState = !ledState;
            }catch(const std::exception &e){
                Logger::logWarningFrom(getDeviceName(), "Failed to read data.");
                Logger::logDebugFrom(getDeviceName(), e.what());
            }       
        }else{
            Io::gpioWrite(pin, Io::GPIO_HIGH);
            ledState = true;
        }

        // Instead of running this every 50ms (using scheduler repeated task)
        // ensure 500ms between end of previous run and start of next
        // this helps to reduce CPU usage (especially on pi zero)
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}
