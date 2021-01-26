package arpirobot.devices.l298n;

import arpirobot.Bridge;
import arpirobot.core.device.MotorController;

public class L298NMotor extends MotorController {

    /**
     * @param in1Pin pin for side 1 directional control
     * @param in2Pin pin for side 2 directional control
     * @param pwmPin pin for pwm of motor
     */
    public L298NMotor(int in1Pin, int in2Pin, int pwmPin){
        ptr = Bridge.arpirobot.L298NMotor_create(in1Pin, in2Pin, pwmPin);
    }

    @Override
    protected void doDestroy() {
        Bridge.arpirobot.L298NMotor_destroy(ptr);
    }
}