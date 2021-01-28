package arpirobot.devices.adafruitmotorhat;

import arpirobot.Bridge;
import arpirobot.core.device.MotorController;

/**
 * Motor for Adafruit Motor Hat
 */
public class AdafruitMotorHatMotor extends MotorController {

    public static final int DETECT_ADDR = -1;
    public static final int ADAFRUIT_ADDR = 0x60;
    public static final int GEEKWORK_ADDR = 0x6F;

    /**
     * @param motorNum The number for the motor
     */
    public AdafruitMotorHatMotor(int motorNum){
        this(motorNum, DETECT_ADDR);
    }

    /**
     * @param motorNum The number for the motor
     * @param address The address of the hat for the motor
     */
    public AdafruitMotorHatMotor(int motorNum, int address){
        this(motorNum, address, true);
    }

    /**
     * @param motorNum The number for the motor
     * @param address The address of the hat for the motor
     * @param remapNumbers If true the motor numbers will be remapped (if needed) to match the 
     *                     physical order of the Adafruit motor hat
     */
    public AdafruitMotorHatMotor(int motorNum, int address, boolean remapNumbers){
        ptr = Bridge.arpirobot.AdafruitMotorHatMotor_create(motorNum, address, remapNumbers);
    }

    @Override
    protected void doDestroy() {
        Bridge.arpirobot.AdafruitMotorHatMotor_destroy(ptr);
    }
}
