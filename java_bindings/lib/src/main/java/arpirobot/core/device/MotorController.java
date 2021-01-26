package arpirobot.core.device;

import arpirobot.Bridge;

/**
 * Base class for motor controllers.
 */
public abstract class MotorController extends BaseDevice {

    /**
     * Check if the motor direction is inverted (positive and negative speed switched)
     * @returns true if inverted, else false
     */
    public boolean isInverted(){
        return Bridge.arpirobot.MotorController_isInverted(ptr);
    }

    /**
     * Set the motor direction to inverted or non-inverted (positive and negative speed switched)
     * @param inverted True for inverted, false for non-inverted
     */
    public void setInverted(boolean inverted){
        Bridge.arpirobot.MotorController_setInverted(ptr, inverted);
    }

    /**
     * Check if brake mode is enabled. When enabled motors with a speed of zero will resist motion.
     * @returns true if enabled, else false
     */
    public boolean isBrakeMode(){
        return Bridge.arpirobot.MotorController_isBrakeMode(ptr);
    }

    /**
     * Set if brake mode is enabled
     * @param brakeMode true for enabled, false for disabled
     */
    public void setBrakeMode(boolean brakeMode){
        Bridge.arpirobot.MotorController_setBrakeMode(ptr, brakeMode);
    }

    /**
     * Get the current speed of the motor
     * @returns the motor's speed
     */
    public double getSpeed(){
        return Bridge.arpirobot.MotorController_getSpeed(ptr);
    }

    /**
     * Set the current speed of the motor (no effect if motor is disabled)
     * @param speed The motor's speed (between -1.0 and 1.0)
     */
    public void setSpeed(double speed){
        Bridge.arpirobot.MotorController_setSpeed(ptr, speed);
    }
}
