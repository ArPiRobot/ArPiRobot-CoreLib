package arpirobot.arduino.sensor;

import arpirobot.Bridge;
import arpirobot.arduino.device.ArduinoDevice;

/**
 *MPU6050 IMU
 */
public class Mpu6050Imu extends ArduinoDevice {
    public Mpu6050Imu(){
        this(true, -1);
    }

    /**
     * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
     * @param deviceId Set this to the hard-coded deviceId if createDevice is false
     */
    public Mpu6050Imu(boolean createDevice, int deviceId){
        ptr = Bridge.arpirobot.Mpu6050Imu_create(createDevice, deviceId);
    }

    @Override
    protected void doDestroy() {
        Bridge.arpirobot.Mpu6050Imu_destroy(ptr);
    }

    /**
     * Calibrate the IMU. Should reduce gyro drift and accelerometer error
     * IMU MUST BE STATIONARY DURING CALIBRATION. 
     * GRAVITATIONAL ACCELERATION MUST BE IN NEGATIVE Z DIRECTION DURING CALIBRATION.
     * @param samples The number of samples to take during calibration. 
     *                More samples may be more accurate, but will take longer.
     */
    public void calibrate(int samples){
        verifyNotDestroyed();
        Bridge.arpirobot.Mpu6050Imu_calibrate(ptr, samples);
    }

    /**
     * Get the X rotation
     * @return X rotation in degrees
     */
    public double getGyroX(){
        verifyNotDestroyed();
        return Bridge.arpirobot.Mpu6050Imu_getGyroX(ptr);
    }

    /**
     * Get the Y rotation
     * @return Y rotation in degrees
     */
    public double getGyroY(){
        verifyNotDestroyed();
        return Bridge.arpirobot.Mpu6050Imu_getGyroY(ptr);
    }

    /**
     * Get the Z rotation
     * @return Z rotation in degrees
     */
    public double getGyroZ(){
        verifyNotDestroyed();
        return Bridge.arpirobot.Mpu6050Imu_getGyroZ(ptr);
    }

    /**
     * Get the X axis acceleration
     * @return X acceleration in m/s^2
     */
    public double getAccelX(){
        verifyNotDestroyed();
        return Bridge.arpirobot.Mpu6050Imu_getAccelX(ptr);
    }

    /**
     * Get the Y axis acceleration
     * @return Y acceleration in m/s^2
     */
    public double getAccelY(){
        verifyNotDestroyed();
        return Bridge.arpirobot.Mpu6050Imu_getAccelY(ptr);
    }

    /**
     * Get the Z axis acceleration
     * @return Z acceleration in m/s^2
     */
    public double getAccelZ(){
        verifyNotDestroyed();
        return Bridge.arpirobot.Mpu6050Imu_getAccelZ(ptr);
    }

    /**
     * Set current X rotation to the given value
     * @param newGyroX The new X rotation to set
     */
    public void setGyroX(double newGyroX){
        verifyNotDestroyed();
        Bridge.arpirobot.Mpu6050Imu_setGyroX(ptr, newGyroX);
    }

    /**
     * Set current Y rotation to the given value
     * @param newGyroY The new Y rotation to set
     */
    public void setGyroY(double newGyroY){
        verifyNotDestroyed();
        Bridge.arpirobot.Mpu6050Imu_setGyroY(ptr, newGyroY);
    }

    /**
     * Set current Z rotation to the given value
     * @param newGyroZ The new Z rotation to set
     */
    public void setGyroZ(double newGyroZ){
        verifyNotDestroyed();
        Bridge.arpirobot.Mpu6050Imu_setGyroZ(ptr, newGyroZ);
    }

}
