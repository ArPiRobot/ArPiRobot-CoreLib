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

package arpirobot.arduino.sensor;

import arpirobot.Bridge;
import arpirobot.arduino.device.ArduinoDevice;

/**
 * Infrared reflection detector module
 */
public class IRReflectorModule extends ArduinoDevice {

    /**
     * @param digitalPin The digital output pin for this sensor
     */
    public IRReflectorModule(int digitalPin){
        this("" + digitalPin);
    }

    /**
     * @param digitalPin The digital output pin for this sensor (prefix with letter A for analog pin)
     */
    public IRReflectorModule(String digitalPin){
        this(digitalPin, "");
    }

    /**
     * @param digitalPin The digital output pin for this sensor
     * @param analogPin The analog output pin for this sensor (can be prefixed with letter A, but not required)
     */
    public IRReflectorModule(int digitalPin, String analogPin){
        this("" + digitalPin, "" + analogPin);
    }

    /**
     * @param digitalPin The digital output pin for this sensor (prefix with letter A for analog pin)
     * @param analogPin The analog output pin for this sensor (can be prefixed with letter A, but not required)
     */
    public IRReflectorModule(String digitalPin, String analogPin){
        this(digitalPin, analogPin, true, -1);
    }

    /**
     * @param digitalPin The digital output pin for this sensor
     * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
     * @param deviceId Set this to the hard-coded deviceId if createDevice is false
     */
    public IRReflectorModule(int digitalPin, boolean createDevice, int deviceId){
        this("" + digitalPin, createDevice, deviceId);
    }

    /**
     * @param digitalPin The digital output pin for this sensor
     * @param analogPin The analog output pin for this sensor (can be prefixed with letter A, but not required)
     * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
     * @param deviceId Set this to the hard-coded deviceId if createDevice is false
     */
    public IRReflectorModule(int digitalPin, String analogPin, boolean createDevice, int deviceId){
        this("" + digitalPin, analogPin, createDevice, deviceId);
    }

    /**
     * @param digitalPin The digital output pin for this sensor (prefix with letter A for analog pin)
     * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
     * @param deviceId Set this to the hard-coded deviceId if createDevice is false
     */
    public IRReflectorModule(String digitalPin, boolean createDevice, int deviceId){
        this(digitalPin, "", createDevice, deviceId);
    }

    /**
     * @param digitalPin The digital output pin for this sensor (prefix with letter A for analog pin)
     * @param analogPin The analog output pin for this sensor (can be prefixed with letter A, but not required)
     * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
     * @param deviceId Set this to the hard-coded deviceId if createDevice is false
     */
    public IRReflectorModule(String digitalPin, String analogPin, boolean createDevice, int deviceId){
        ptr = Bridge.arpirobot.IRReflectorModule_create(digitalPin, analogPin, createDevice, deviceId);
    }

    @Override
    protected void doDestroy() {
        Bridge.arpirobot.IRReflectorModule_destroy(ptr);
    }

    /**
     * Get the digital value read from the sensor
     * @return true or false. One cooresponds to reflection detected. The other is no reflection detected.
     */
    public boolean getDigitalValue(){
        verifyNotDestroyed();
        return Bridge.arpirobot.IRReflectorModule_getDigitalValue(ptr);
    }

    /**
     * Get the analog reflection signal strength (if this device was setup to use an analog input)
     * @return The read analog value (will be zero if not configured for analog data)
     */
    public int getAnalogValue(){
        verifyNotDestroyed();
        return Bridge.arpirobot.IRReflectorModule_getAnalogValue(ptr);
    }
}
