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
