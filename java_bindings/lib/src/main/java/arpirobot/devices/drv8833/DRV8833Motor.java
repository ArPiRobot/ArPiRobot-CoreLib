package arpirobot.devices.drv8833;

import arpirobot.Bridge;
import arpirobot.core.device.MotorController;

/** 
 * Motor on DRV8833 module
 */
public class DRV8833Motor extends MotorController {
    /**
     * @param in1Pin pin for side 1 directional control
     * @param in2Pin pin for side 2 directional control
     * @param slpPin pin for sleep of module
     */
    public DRV8833Motor(int in1Pin, int in2Pin, int slpPin){
        ptr = Bridge.arpirobot.DRV8833Motor_create(in1Pin, in2Pin, slpPin);
    }

    @Override
    protected void doDestroy() {
        Bridge.arpirobot.DRV8833Motor_destroy(ptr);
    }
}
