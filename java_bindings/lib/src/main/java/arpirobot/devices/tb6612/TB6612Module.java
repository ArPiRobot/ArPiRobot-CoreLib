package arpirobot.devices.tb6612;

/**
 * TB6612 Module (two motors)
 */
public class TB6612Module {
    private TB6612Motor motorA, motorB;

    /**
     * @param aIn1Pin In1 pin for motor a of module
     * @param aIn2Pin In2 pin for motor a of module
     * @param pwmaPin PWM pin for motor a of module
     * @param bIn1Pin In1 pin for motor b of module
     * @param bIn2Pin In2 pin for motor b of module
     * @param pwmbPin PWM pin for motor b of module
     */
    public TB6612Module(int aIn1Pin, int aIn2Pin, int pwmaPin, int bIn1Pin, int bIn2Pin, int pwmbPin){
        motorA = new TB6612Motor(aIn1Pin, aIn2Pin, pwmaPin);
        motorB = new TB6612Motor(bIn1Pin, bIn2Pin, pwmbPin);
    }

    /**
     * Get the Motor instance for motor a
     */
    public TB6612Motor getMotorA(){
        return motorA;
    }

    /**
     * Get the Motor instance for motor b
     */
    public TB6612Motor getMotorB(){
        return motorB;
    }
}
