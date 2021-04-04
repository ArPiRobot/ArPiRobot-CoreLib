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

package arpirobot.devices.tb6612;

/**
 * TB6612 Module (two motors)
 */
public class TB6612Module {
    private TB6612Motor motorA, motorB;

    /**
     * @param aIn1Pin In1 pin for motor a of module
     * @param aIn2Pin In2 pin for motor a of module
     * @param pwmaPin PWM pin for motor a of module
     * @param bIn1Pin In1 pin for motor b of module
     * @param bIn2Pin In2 pin for motor b of module
     * @param pwmbPin PWM pin for motor b of module
     */
    public TB6612Module(int aIn1Pin, int aIn2Pin, int pwmaPin, int bIn1Pin, int bIn2Pin, int pwmbPin){
        motorA = new TB6612Motor(aIn1Pin, aIn2Pin, pwmaPin);
        motorB = new TB6612Motor(bIn1Pin, bIn2Pin, pwmbPin);
    }

    /**
     * Get the Motor instance for motor a
     */
    public TB6612Motor getMotorA(){
        return motorA;
    }

    /**
     * Get the Motor instance for motor b
     */
    public TB6612Motor getMotorB(){
        return motorB;
    }
}
