package arpirobot.devices.drv8833;

/**
 * DRV8833 Module (two motors)
 */
public class DRV8833Module {
    private DRV8833Motor motorA, motorB;

    /**
     * @param aIn1Pin In1 pin for motor a of module
     * @param aIn2Pin In2 pin for motor a of module
     * @param bIn1Pin In1 pin for motor b of module
     * @param bIn2Pin In2 pin for motor b of module
     * @param slpPin Sleep pin for module
     */
    public DRV8833Module(int aIn1Pin, int aIn2Pin, int bIn1Pin, int bIn2Pin, int slpPin){
        motorA = new DRV8833Motor(aIn1Pin, aIn2Pin, slpPin);
        motorB = new DRV8833Motor(bIn1Pin, bIn2Pin, slpPin);
    }

    /**
     * Get the Motor instance for motor a
     */
    public DRV8833Motor getMotorA(){
        return motorA;
    }

    /**
     * Get the Motor instance for motor b
     */
    public DRV8833Motor getMotorB(){
        return motorB;
    }
}
