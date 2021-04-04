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

#include <arpirobot/devices/l298n/L298NModule.hpp>
#include <arpirobot/devices/l298n/L298NMotor.hpp>

using namespace arpirobot;


L298NModule::L298NModule(int in1Pin, int in2Pin, int enaPin, int in3Pin, int in4Pin, int enbPin) : 
        motorA(in1Pin, in2Pin, enaPin), motorB(in3Pin, in4Pin, enbPin){
    
}

L298NMotor &L298NModule::getMotorA(){
    return motorA;
}

L298NMotor &L298NModule::getMotorB(){
    return motorB;
}
