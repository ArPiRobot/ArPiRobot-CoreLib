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

package arpirobot.arduino.device;

import arpirobot.Bridge;
import arpirobot.DestroyableObject;

import com.sun.jna.Pointer;

public abstract class ArduinoDevice extends DestroyableObject {

    /**
     * Get the human-readable name of the device
     * @return the devices human readable name
     */
    public String getDeviceName(){
        verifyNotDestroyed();
        Pointer strPtr = Bridge.arpirobot.ArduinoDevice_getDeviceName(ptr);
        String str = strPtr.getString(0);
        Bridge.arpirobot.freeString(strPtr);
        return str;
    }
}
