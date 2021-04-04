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

package arpirobot.arduino.iface;

import arpirobot.DestroyableObject;
import arpirobot.arduino.device.ArduinoDevice;
import arpirobot.Bridge;

/** 
 * Common base class (communication protocol agnostic) for an arduino (or other microcontroller) sensor coprocessor.
 * This class handles all the high-level data between the arduino and pi, without implementing a specific
 * communication protocol. See the below classes for different communication protocols.
 */
public abstract class BaseArduinoInterface extends DestroyableObject {

    /**
     * Create devices (as needed) and start processing sensor data. After this is called devices can 
     * no longer be added to this arduino using BaseArduinoInterface::addDevice.
     */
    public void begin(){
        verifyNotDestroyed();
        Bridge.arpirobot.BaseArduinoInterface_begin(ptr);
    }

    /**
     * Add a device to this arduino interface instance. 
     * You cannot add devices after BaseArduinoInterface::begin is called
     * @param device The ArduinoDevice instance to add
     */
    public void addDevice(ArduinoDevice device){
        verifyNotDestroyed();
        Bridge.arpirobot.BaseArduinoInterface_addDevice(ptr, device._ptr());
    }

    /**
     * Returns true when the arduino is ready to process sensor data (after BaseArduinoInterface::begin 
     * is successful)
     * @return true if ready, else false
     */
    public boolean isReady(){
        verifyNotDestroyed();
        return Bridge.arpirobot.BaseArduinoInterface_isReady(ptr);
    }
}
