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

package arpirobot.devices.l298n;

/**
 * L298N Module (two motors)
 */
public class L298NModule {
    private L298NMotor motorA, motorB;

    /**
     * @param in1Pin In1 pin for module (motor a)
     * @param in2Pin In2 pin for module (motor a)
     * @param enaPin PWM pin for motor a of module
     * @param in3Pin In3 pin for module (motor b)
     * @param in4Pin In4 pin for module (motor b)
     * @param enbPin PWM pin for motor b of module
     */
    public L298NModule(int in1Pin, int in2Pin, int enaPin, int in3Pin, int in4Pin, int enbPin){
        motorA = new L298NMotor(in1Pin, in2Pin, enaPin);
        motorB = new L298NMotor(in3Pin, in4Pin, enbPin);
    }

    /**
     * Get the Motor instance for motor a
     */
    public L298NMotor getMotorA(){
        return motorA;
    }

    /**
     * Get the Motor instance for motor b
     */
    public L298NMotor getMotorB(){
        return motorB;
    }
}
