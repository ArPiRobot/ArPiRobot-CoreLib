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


#include <arpirobot/devices/drv8833/DRV8833Module.hpp>
#include <arpirobot/devices/drv8833/DRV8833Motor.hpp>


using namespace arpirobot;

DRV8833Module::DRV8833Module(int aIn1Pin, int aIn2Pin, int bIn1Pin, int bIn2Pin, int slpPin) : 
        motorA(aIn1Pin, aIn2Pin, slpPin), motorB(bIn1Pin, bIn2Pin, slpPin){
    
}

DRV8833Motor &DRV8833Module::getMotorA(){
    return motorA;
}

DRV8833Motor &DRV8833Module::getMotorB(){
    return motorB;
}