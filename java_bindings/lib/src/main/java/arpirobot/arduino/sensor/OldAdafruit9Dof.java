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
 * Now discontinued Adafruit 9-DOF IMU
 * https://www.adafruit.com/product/1714
 */
public class OldAdafruit9Dof extends ArduinoDevice {
    public OldAdafruit9Dof(){
        this(true, -1);
    }

    /**
     * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
     * @param deviceId Set this to the hard-coded deviceId if createDevice is false
     */
    public OldAdafruit9Dof(boolean createDevice, int deviceId){
        ptr = Bridge.arpirobot.OldAdafruit9Dof_create(createDevice, deviceId);
    }

    @Override
    protected void doDestroy() {
        Bridge.arpirobot.OldAdafruit9Dof_destroy(ptr);
    }

    /**
     * Get the X rotation
     * @return X rotation in degrees
     */
    public double getGyroX(){
        verifyNotDestroyed();
        return Bridge.arpirobot.OldAdafruit9Dof_getGyroX(ptr);
    }

    /**
     * Get the Y rotation
     * @return Y rotation in degrees
     */
    public double getGyroY(){
        verifyNotDestroyed();
        return Bridge.arpirobot.OldAdafruit9Dof_getGyroY(ptr);
    }

    /**
     * Get the Z rotation
     * @return Z rotation in degrees
     */
    public double getGyroZ(){
        verifyNotDestroyed();
        return Bridge.arpirobot.OldAdafruit9Dof_getGyroZ(ptr);
    }

    /**
     * Get the X axis acceleration
     * @return X acceleration in m/s^2
     */
    public double getAccelX(){
        verifyNotDestroyed();
        return Bridge.arpirobot.OldAdafruit9Dof_getAccelX(ptr);
    }

    /**
     * Get the Y axis acceleration
     * @return Y acceleration in m/s^2
     */
    public double getAccelY(){
        verifyNotDestroyed();
        return Bridge.arpirobot.OldAdafruit9Dof_getAccelY(ptr);
    }

    /**
     * Get the Z axis acceleration
     * @return Z acceleration in m/s^2
     */
    public double getAccelZ(){
        verifyNotDestroyed();
        return Bridge.arpirobot.OldAdafruit9Dof_getAccelZ(ptr);
    }

    /**
     * Set current X rotation to the given value
     * @param newGyroX The new X rotation to set
     */
    public void setGyroX(double newGyroX){
        verifyNotDestroyed();
        Bridge.arpirobot.OldAdafruit9Dof_setGyroX(ptr, newGyroX);
    }

    /**
     * Set current Y rotation to the given value
     * @param newGyroY The new Y rotation to set
     */
    public void setGyroY(double newGyroY){
        verifyNotDestroyed();
        Bridge.arpirobot.OldAdafruit9Dof_setGyroY(ptr, newGyroY);
    }

    /**
     * Set current Z rotation to the given value
     * @param newGyroZ The new Z rotation to set
     */
    public void setGyroZ(double newGyroZ){
        verifyNotDestroyed();
        Bridge.arpirobot.OldAdafruit9Dof_setGyroZ(ptr, newGyroZ);
    }

}
