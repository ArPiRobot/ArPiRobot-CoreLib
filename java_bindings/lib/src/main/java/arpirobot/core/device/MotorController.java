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

package arpirobot.core.device;

import arpirobot.Bridge;

/**
 * Base class for motor controllers.
 */
public abstract class MotorController extends BaseDevice {

    /**
     * Check if the motor direction is inverted (positive and negative speed switched)
     * @return true if inverted, else false
     */
    public boolean isInverted(){
        verifyNotDestroyed();
        return Bridge.arpirobot.MotorController_isInverted(ptr);
    }

    /**
     * Set the motor direction to inverted or non-inverted (positive and negative speed switched)
     * @param inverted True for inverted, false for non-inverted
     */
    public void setInverted(boolean inverted){
        verifyNotDestroyed();
        Bridge.arpirobot.MotorController_setInverted(ptr, inverted);
    }

    /**
     * Check if brake mode is enabled. When enabled motors with a speed of zero will resist motion.
     * @return true if enabled, else false
     */
    public boolean isBrakeMode(){
        verifyNotDestroyed();
        return Bridge.arpirobot.MotorController_isBrakeMode(ptr);
    }

    /**
     * Set if brake mode is enabled
     * @param brakeMode true for enabled, false for disabled
     */
    public void setBrakeMode(boolean brakeMode){
        verifyNotDestroyed();
        Bridge.arpirobot.MotorController_setBrakeMode(ptr, brakeMode);
    }

    /**
     * Get the current speed of the motor
     * @return the motor's speed
     */
    public double getSpeed(){
        verifyNotDestroyed();
        return Bridge.arpirobot.MotorController_getSpeed(ptr);
    }

    /**
     * Set the current speed of the motor (no effect if motor is disabled)
     * @param speed The motor's speed (between -1.0 and 1.0)
     */
    public void setSpeed(double speed){
        verifyNotDestroyed();
        Bridge.arpirobot.MotorController_setSpeed(ptr, speed);
    }
}
