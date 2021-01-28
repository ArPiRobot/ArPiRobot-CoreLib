package arpirobot.core.drive;

import arpirobot.Bridge;
import arpirobot.DestroyableObject;
import arpirobot.core.device.MotorController;

import com.sun.jna.Pointer;

/**
 * Drive helper for tank drive method.
 * 
 * Drive helpers calculate speeds for each motor based on a set of inputs and update all motors speeds at once.
 * Tank drive uses two inputs: left speed and right speed.
 * 
 */

public class TankDriveHelper extends DestroyableObject {
    
    public TankDriveHelper(MotorController leftMotor, MotorController rightMotor){
        this(new MotorController[]{leftMotor}, new MotorController[]{rightMotor});
    }

    public TankDriveHelper(MotorController[] leftMotors, MotorController[] rightMotors){
        Pointer[] lmInternal = new Pointer[leftMotors.length];
        Pointer[] rmInternal = new Pointer[rightMotors.length];

        // Build arrays of internal pointers
        for(int i = 0; i < leftMotors.length; ++i){
            lmInternal[i] = leftMotors[i]._ptr();
        }
        for(int i = 0; i < rightMotors.length; ++i){
            rmInternal[i] = rightMotors[i]._ptr();
        }

        //ptr = Bridge.arpirobot.TankDriveHelper_create(lmInternal, lmInternal.length, rmInternal, rmInternal.length);
    }

    @Override
    protected void doDestroy() {
        Bridge.arpirobot.TankDriveHelper_destroy(ptr);
    }

    /**
     * Set a new left speed value. Speeds will be re-calculated and updated for each motor
     * @param newLeftSpeed the new rotation (-1.0 to 1.0)
     */
    void updateLeftSpeed(double newLeftSpeed){
        verifyNotDestroyed();
        Bridge.arpirobot.TankDriveHelper_updateLeftSpeed(ptr, newLeftSpeed);
    }

    /**
     * Set a new right speed value. Speeds will be re-calculated and updated for each motor
     * @param newRightSpeed the new rotation (-1.0 to 1.0)
     */
    void updateRightSpeed(double newRightSpeed){
        verifyNotDestroyed();
        Bridge.arpirobot.TankDriveHelper_updateRightSpeed(ptr, newRightSpeed);
    }

    /**
     * Set a new left and right speed value at the same time. 
     * Speeds will be re-calculated and updated for each motor.
     * If updating both values use this instead of calling updateLeftSpeed and updateRightSpeed
     * because this will only recalculate motor speeds once.
     * @param newLeftSpeed the new speed (-1.0 to 1.0)
     * @param newRightSpeed the new rotation (-1.0 to 1.0)
     */
    void update(double newLeftSpeed, double newRightSpeed){
        verifyNotDestroyed();
        Bridge.arpirobot.TankDriveHelper_update(ptr, newLeftSpeed, newRightSpeed);
    }

}
