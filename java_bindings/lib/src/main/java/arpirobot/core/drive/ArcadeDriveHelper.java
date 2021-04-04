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

package arpirobot.core.drive;

import arpirobot.Bridge;
import arpirobot.DestroyableObject;
import arpirobot.core.device.MotorController;

import com.sun.jna.Pointer;

/**
 * Drive helper for arcade drive method.
 * 
 * Drive helpers calculate speeds for each motor based on a set of inputs and update all motors speeds at once.
 * Arcade drive uses two inputs: speed and rotation.
 * 
 */
public class ArcadeDriveHelper extends DestroyableObject {

    /**
     * @param leftMotor A singular left side motor
     * @param rightMotor A singular right side motor
     */
    public ArcadeDriveHelper(MotorController leftMotor, MotorController rightMotor){
        this(new MotorController[]{leftMotor}, new MotorController[]{rightMotor});
    }

    /**
     * @param leftMotors A set of left motors
     * @param rightMotors A set of right motors
     */
    public ArcadeDriveHelper(MotorController[] leftMotors, MotorController[] rightMotors){
        Pointer[] lmInternal = new Pointer[leftMotors.length];
        Pointer[] rmInternal = new Pointer[rightMotors.length];

        // Build arrays of internal pointers
        for(int i = 0; i < leftMotors.length; ++i){
            lmInternal[i] = leftMotors[i]._ptr();
        }
        for(int i = 0; i < rightMotors.length; ++i){
            rmInternal[i] = rightMotors[i]._ptr();
        }

        ptr = Bridge.arpirobot.ArcadeDriveHelper_create(Bridge.ptrArrayToPtr(lmInternal), lmInternal.length, 
                Bridge.ptrArrayToPtr(rmInternal), rmInternal.length);
    }

    @Override
    protected void doDestroy() {
        Bridge.arpirobot.ArcadeDriveHelper_destroy(ptr);
    }

    /**
     * Set a new speed value. Speeds will be re-calculated and updated for each motor
     * @param newSpeed the new speed (-1.0 to 1.0)
     */
    public void updateSpeed(double newSpeed){
        verifyNotDestroyed();
        Bridge.arpirobot.ArcadeDriveHelper_updateSpeed(ptr, newSpeed);
    }

    /**
     * Set a new rotation value. Speeds will be re-calculated and updated for each motor
     * @param newRotation the new rotation (-1.0 to 1.0)
     */
    public void updateRotation(double newRotation){
        verifyNotDestroyed();
        Bridge.arpirobot.ArcadeDriveHelper_updateRotation(ptr, newRotation);
    }

    /**
     * Set a new speed and rotation value at the same time. 
     * Speeds will be re-calculated and updated for each motor.
     * If updating both values use this instead of calling updateSpeed and updateRotation
     * because this will only recalculate motor speeds once.
     * @param newSpeed the new speed (-1.0 to 1.0)
     * @param newRotation the new rotation (-1.0 to 1.0)
     */
    public void update(double newSpeed, double newRotation){
        verifyNotDestroyed();
        Bridge.arpirobot.ArcadeDriveHelper_update(ptr, newSpeed, newRotation);
    }

}
