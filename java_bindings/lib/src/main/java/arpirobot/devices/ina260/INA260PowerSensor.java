package arpirobot.devices.ina260;

import arpirobot.Bridge;
import arpirobot.core.device.BaseDevice;

/**
 * INA260 Power sensor
 */
public class INA260PowerSensor extends BaseDevice {
    
    public INA260PowerSensor(){
        ptr = Bridge.arpirobot.INA260PowerSensor_create();
    }

    @Override
    protected void doDestroy() {
        Bridge.arpirobot.INA260PowerSensor_destroy(ptr);
    }

    /**
     * Get current measurement
     * @return Current in mA
     */
    public double getCurrent(){
        verifyNotDestroyed();
        return Bridge.arpirobot.INA260PowerSensor_getCurrent(ptr);
    }

    /**
     * Get voltage measurement
     * @return Voltage in V
     */
    public double getVolgate(){
        verifyNotDestroyed();
        return Bridge.arpirobot.INA260PowerSensor_getVoltage(ptr);
    }

    /**
     * Get power measurement
     * @return Power in mW
     */
    public double getPower(){
        verifyNotDestroyed();
        return Bridge.arpirobot.INA260PowerSensor_getPower(ptr);
    }

}
