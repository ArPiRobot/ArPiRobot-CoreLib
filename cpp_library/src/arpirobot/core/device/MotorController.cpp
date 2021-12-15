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

#include <arpirobot/core/device/MotorController.hpp>


using namespace arpirobot;


bool MotorController::isInverted(){
    return speedFactor == -1;
}

void MotorController::setInverted(bool inverted){
    speed *= speedFactor; // Multiply by old factor (back to base speed)
    speedFactor = inverted ? -1 : 1; // Change factor
    speed *= speedFactor; // Multiply by new factor
}

bool MotorController::isBrakeMode(){
    return brakeMode;
}

void MotorController::setBrakeMode(bool brakeMode){
    this->brakeMode = brakeMode;

    // Update brake mode even if motor is disabled
    {
        std::lock_guard<std::mutex> l(lock);
        run();
    }
}

double MotorController::getSpeed(){
    if(!enabled)
        return 0;
    {
        std::lock_guard<std::mutex> l(lock);
        return speed / speedFactor; // divide by speed factor so this matches what user sets
    }
}

void MotorController::setSpeed(double speed){
    if(!enabled)
        return;
    
    // Limit range of speeds
    if(speed > 1)
        speed = 1;
    if(speed < -1)
        speed = -1;

    {
        std::lock_guard<std::mutex> l(lock);
        this->speed = speed * speedFactor;
        run();
    }
}

bool MotorController::isEnabled(){
    return enabled;
}

bool MotorController::shouldMatchRobotState(){
    return true;
}

bool MotorController::shouldDisableWithWatchdog(){
    return true;
}

void MotorController::enable(){
    if(!initialized)
        return;
    {
        std::lock_guard<std::mutex> l(lock);
        enabled = true;
    }
}

void MotorController::disable(){
    if(!initialized)
        return;
    {
        std::lock_guard<std::mutex> l(lock);
        enabled = false;
        speed = 0;
        run();
    }
}
