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


#include <arpirobot/devices/tb6612/TB6612Module.hpp>
#include <arpirobot/devices/tb6612/TB6612Motor.hpp>

using namespace arpirobot;


TB6612Module::TB6612Module(int aIn1Pin, int aIn2Pin, int pwmaPin, int bIn1Pin, int bIn2Pin, int pwmbPin) : 
        motorA(aIn1Pin, aIn2Pin, pwmaPin), motorB(bIn1Pin, bIn2Pin, pwmbPin){
    
}

TB6612Motor &TB6612Module::getMotorA(){
    return motorA;
}

TB6612Motor &TB6612Module::getMotorB(){
    return motorB;
}
