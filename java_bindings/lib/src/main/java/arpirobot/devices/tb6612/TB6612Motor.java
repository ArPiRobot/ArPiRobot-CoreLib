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
