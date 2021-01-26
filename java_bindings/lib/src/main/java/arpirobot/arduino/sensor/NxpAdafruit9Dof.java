package arpirobot.arduino.sensor;

import arpirobot.Bridge;
import arpirobot.arduino.device.ArduinoDevice;

/**
 * Newer adafruit 9-DOF NXP IMU
 */
public class NxpAdafruit9Dof extends ArduinoDevice {
    public NxpAdafruit9Dof(){
        this(true, -1);
    }

    /**
     * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
     * @param deviceId Set this to the hard-coded deviceId if createDevice is false
     */
    public NxpAdafruit9Dof(boolean createDevice, int deviceId){
        ptr = Bridge.arpirobot.NxpAdafruit9Dof_create(createDevice, deviceId);
    }

    @Override
    protected void doDestroy() {
        Bridge.arpirobot.NxpAdafruit9Dof_destroy(ptr);
    }

    /**
     * Get the X rotation
     * @return X rotation in degrees
     */
    public double getGyroX(){
        verifyNotDestroyed();
        return Bridge.arpirobot.NxpAdafruit9Dof_getGyroX(ptr);
    }

    /**
     * Get the Y rotation
     * @return Y rotation in degrees
     */
    public double getGyroY(){
        verifyNotDestroyed();
        return Bridge.arpirobot.NxpAdafruit9Dof_getGyroY(ptr);
    }

    /**
     * Get the Z rotation
     * @return Z rotation in degrees
     */
    public double getGyroZ(){
        verifyNotDestroyed();
        return Bridge.arpirobot.NxpAdafruit9Dof_getGyroZ(ptr);
    }

    /**
     * Get the X axis acceleration
     * @return X acceleration in m/s^2
     */
    public double getAccelX(){
        verifyNotDestroyed();
        return Bridge.arpirobot.NxpAdafruit9Dof_getAccelX(ptr);
    }

    /**
     * Get the Y axis acceleration
     * @return Y acceleration in m/s^2
     */
    public double getAccelY(){
        verifyNotDestroyed();
        return Bridge.arpirobot.NxpAdafruit9Dof_getAccelY(ptr);
    }

    /**
     * Get the Z axis acceleration
     * @return Z acceleration in m/s^2
     */
    public double getAccelZ(){
        verifyNotDestroyed();
        return Bridge.arpirobot.NxpAdafruit9Dof_getAccelZ(ptr);
    }

    /**
     * Set current X rotation to the given value
     * @param newGyroX The new X rotation to set
     */
    public void setGyroX(double newGyroX){
        verifyNotDestroyed();
        Bridge.arpirobot.NxpAdafruit9Dof_setGyroX(ptr, newGyroX);
    }

    /**
     * Set current Y rotation to the given value
     * @param newGyroY The new Y rotation to set
     */
    public void setGyroY(double newGyroY){
        verifyNotDestroyed();
        Bridge.arpirobot.NxpAdafruit9Dof_setGyroY(ptr, newGyroY);
    }

    /**
     * Set current Z rotation to the given value
     * @param newGyroZ The new Z rotation to set
     */
    public void setGyroZ(double newGyroZ){
        verifyNotDestroyed();
        Bridge.arpirobot.NxpAdafruit9Dof_setGyroZ(ptr, newGyroZ);
    }

}
