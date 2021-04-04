/*
 * Copyright 2021 Marcus Behel
 *
 * This file is part of ArPiRobot-CoreLib.
 * 
 * ArPiRobot-CoreLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ArPiRobot-CoreLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with ArPiRobot-CoreLib.  If not, see <https://www.gnu.org/licenses/>. 
 */

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
