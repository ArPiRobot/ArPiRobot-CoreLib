package arpirobot.core.drive;

import arpirobot.Bridge;
import arpirobot.DestroyableObject;
import arpirobot.core.device.MotorController;

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
        long[] lmInternal = new long[leftMotors.length];
        long[] rmInternal = new long[rightMotors.length];

        // Build arrays of internal pointers
        for(int i = 0; i < leftMotors.length; ++i){
            lmInternal[i] = leftMotors[i]._ptr();
        }
        for(int i = 0; i < rightMotors.length; ++i){
            rmInternal[i] = rightMotors[i]._ptr();
        }

        ptr = Bridge.arpirobot.ArcadeDriveHelper_create(lmInternal, lmInternal.length, rmInternal, rmInternal.length);
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
        Bridge.arpirobot.ArcadeDriveHelper_updateSpeed(ptr, newSpeed);
    }

    /**
     * Set a new rotation value. Speeds will be re-calculated and updated for each motor
     * @param newRotation the new rotation (-1.0 to 1.0)
     */
    public void updateRotation(double newRotation){
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
        Bridge.arpirobot.ArcadeDriveHelper_update(ptr, newSpeed, newRotation);
    }

}