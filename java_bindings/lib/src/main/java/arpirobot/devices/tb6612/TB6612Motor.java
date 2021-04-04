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

import arpirobot.Bridge;
import arpirobot.core.device.MotorController;

/**
 * Motor on TB6612 module
 */
public class TB6612Motor extends MotorController {

    /**
     * @param in1Pin pin for side 1 directional control
     * @param in2Pin pin for side 2 directional control
     * @param pwmPin pin for pwm of motor
     */
    public TB6612Motor(int in1Pin, int in2Pin, int pwmPin){
        ptr = Bridge.arpirobot.TB6612Motor_create(in1Pin, in2Pin, pwmPin);
    }

    @Override
    protected void doDestroy() {
        Bridge.arpirobot.TB6612Motor_destroy(ptr);
    }
}
