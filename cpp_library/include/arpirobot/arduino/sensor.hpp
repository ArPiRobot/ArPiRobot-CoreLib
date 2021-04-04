#pragma once

#include <arpirobot/arduino/device.hpp>
#include <arpirobot/arduino/iface.hpp>
#include <arpirobot/core/network.hpp>

namespace arpirobot{
    /**
     * \class VoltageMonitor sensor.hpp arpirobot/arduino/sensor.hpp
     * 
     * Simple analog input voltage monitor (using voltage divider) connected to arduino analog input.
     */
    class VoltageMonitor : public ArduinoDevice, public MainVmon{
    public:

        /**
         * @param pin The analog input pin this voltage monitor is connected to (can be prefixed with letter A)
         * @param vboard The board voltage for this arduino (usually 5V or 3.3V).
         * @param r1 The top resistor of the voltage divider
         * @param r2 The bottom resistor of the voltage divider (the one voltage is measured across)
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        VoltageMonitor(std::string pin, double vboard, int r1, int r2, bool createDevice = true, int deviceId = -1);

        /**
         * @param pin The analog input pin number this voltage monitor is connected
         * @param vboard The board voltage for this arduino (usually 5V or 3.3V).
         * @param r1 The top resistor of the voltage divider
         * @param r2 The bottom resistor of the voltage divider (the one voltage is measured across)
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        VoltageMonitor(int pin, double vboard, int r1, int r2, bool createDevice = true, int deviceId = -1);

        /**
         * Get the voltage read by this voltage monitor (in volts)
         * @return The voltage in volts
         */
        double getVoltage();

        /**
         * Get the human-readable name for this device
         * @return The device name
         */
        std::string getDeviceName();
    
    protected:
        void applyDefaultState();
        std::vector<uint8_t> getCreateData();
        void handleData(const std::vector<uint8_t> &data);
    
    private:
        std::string pin;
        double vboard;
        int r1;
        int r2;
        double voltage = 0;
    };

    /**
     * \class Ultrasonic4Pin sensor.hpp arpirobot/arduino/sensor.hpp
     * 
     * Device for 4-pin ultrasonic sensor.
     */
    class Ultrasonic4Pin : public ArduinoDevice{
    public:
        /**
         * @param triggerPin Digital pin number for the trigger pin
         * @param echoPin Digital pin number for the echo pin
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        Ultrasonic4Pin(int triggerPin, int echoPin, bool createDevice = true, int deviceId = -1);

        /**
         * @param triggerPin Digital pin number for the trigger pin
         * @param echoPin Pin number for the echo pin (prefix with letter A for analog pin)
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        Ultrasonic4Pin(int triggerPin, std::string echoPin, bool createDevice = true, int deviceId = -1);

        /**
         * @param triggerPin Pin number for the trigger pin (prefix with letter A for analog pin)
         * @param echoPin Digital pin number for the echo pin
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        Ultrasonic4Pin(std::string triggerPin, int echoPin, bool createDevice = true, int deviceId = -1);

        /**
         * @param triggerPin Pin number for the trigger pin (prefix with letter A for analog pin)
         * @param echoPin Pin number for the echo pin (prefix with letter A for analog pin)
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        Ultrasonic4Pin(std::string triggerPin, std::string echoPin, bool createDevice = true, int deviceId = -1);

        /**
         * Get the last distance read by the ultrasonic sensor
         * @return The distance in cm
         */
        int getDistance();

        /**
         * Get the human-readable name for this device
         * @return The device name
         */
        std::string getDeviceName();
    
    protected:
        void applyDefaultState();
        std::vector<uint8_t> getCreateData();
        void handleData(const std::vector<uint8_t> &data);
    
    private:
        std::string triggerPin;
        std::string echoPin;
        int distance = 0;
    };
    
    /**
     * \class SingleEncoder sensor.hpp arpirobot/arduino/sensor.hpp
     * 
     * Single-channel encoder
     */
    class SingleEncoder : public ArduinoDevice{
    public:

        /**
         * @param pin The digital pin number this encoder is connected to
         * @param useInternalPullup Set to true to use the arduino's internal pullup resistor on this pin (if supported)
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        SingleEncoder(int pin, bool useInternalPullup, bool createDevice = true, int deviceId = -1);

        /**
         * @param pin The pin number this encoder is connected to (prefix with letter A for analog pin)
         * @param useInternalPullup Set to true to use the arduino's internal pullup resistor on this pin (if supported)
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        SingleEncoder(std::string pin, bool useInternalPullup, bool createDevice = true, int deviceId = -1);

        /**
         * Get the position (tick count) for this encoder
         * @return The position in ticks
         */
        int getPosition();

        /**
         * Set the current tick count to the given value
         * @param currentPosition The new tick value to set
         */
        void setPosition(int currentPosition);

        /**
         * Get the human-readable name for this device
         * @return The device name
         */
        std::string getDeviceName();
    
    protected:
        void applyDefaultState();
        std::vector<uint8_t> getCreateData();
        void handleData(const std::vector<uint8_t> &data);
    
    private:
        std::string pin;
        uint8_t useInternalPullup;
        int count = 0;
        int countOffset = 0;
    };

    /**
     * \class IRReflectorModule sensor.hpp arpirobot/arduino/sensor.hpp
     * 
     * Infrared reflection detector module
     */
    class IRReflectorModule : public ArduinoDevice{
    public:
        /**
         * @param digitalPin The digital output pin for this sensor
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        IRReflectorModule(int digitalPin, bool createDevice = true, int deviceId = -1);

        /**
         * @param digitalPin The digital output pin for this sensor (prefix with letter A for analog pin)
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        IRReflectorModule(std::string digitalPin, bool createDevice = true, int deviceId = -1);

        /**
         * @param digitalPin The digital output pin for this sensor
         * @param analogPin The analog output pin for this sensor (can be prefixed with letter A, but not required)
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        IRReflectorModule(int digitalPin, std::string analogPin, bool createDevice = true, int deviceId = -1);

        /**
         * @param digitalPin The digital output pin for this sensor (prefix with letter A for analog pin)
         * @param analogPin The analog output pin for this sensor (can be prefixed with letter A, but not required)
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        IRReflectorModule(std::string digitalPin, std::string analogPin, bool createDevice = true, int deviceId = -1);

        /**
         * Get the digital value read from the sensor
         * @return true or false. One cooresponds to reflection detected. The other is no reflection detected.
         */
        bool getDigitalValue();

        /**
         * Get the analog reflection signal strength (if this device was setup to use an analog input)
         * @return The read analog value (will be zero if not configured for analog data)
         */
        int getAnalogValue();

        /**
         * Get the human-readable name for this device
         * @return The device name
         */
        std::string getDeviceName();

    protected:
        void applyDefaultState();
        std::vector<uint8_t> getCreateData();
        void handleData(const std::vector<uint8_t> &data);
    
    private:
        std::string digitalPin;
        std::string analogPin;
        bool digitalValue = false;
        int analogValue = 0;
    };

    /**
     * \class OldAdafruit9Dof sensor.hpp arpirobot/arduino/sensor.hpp
     * 
     * Now discontinued Adafruit 9-DOF IMU
     * https://www.adafruit.com/product/1714
     */
    class OldAdafruit9Dof : public ArduinoDevice{
    public:

        /**
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        OldAdafruit9Dof(bool createDevice = true, int deviceId = -1);

        /**
         * Calibrate the IMU. Should reduce gyro drift and accelerometer error
         * IMU MUST BE STATIONARY DURING CALIBRATION. 
         * GRAVITATIONAL ACCELERATION MUST BE IN NEGATIVE Z DIRECTION DURING CALIBRATION.
         * @param samples The number of samples to take during calibration. 
         *                More samples may be more accurate, but will take longer.
         */
        void calibrate(uint16_t samples);

        /**
         * Get the X rotation
         * @return X rotation in degrees
         */
        double getGyroX();

        /**
         * Get the Y rotation
         * @return Y rotation in degrees
         */
        double getGyroY();

        /**
         * Get the Z rotation
         * @return Z rotation in degrees
         */
        double getGyroZ();

        /**
         * Get the X axis acceleration
         * @return X acceleration in m/s^2
         */
        double getAccelX();

        /**
         * Get the Y axis acceleration
         * @return Y acceleration in m/s^2
         */
        double getAccelY();

        /**
         * Get the Z axis acceleration
         * @return Z acceleration in m/s^2
         */
        double getAccelZ();

        /**
         * Set current X rotation to the given value
         * @param newGyroX The new X rotation to set
         */
        void setGyroX(double newGyroX);

        /**
         * Set current Y rotation to the given value
         * @param newGyroY The new Y rotation to set
         */
        void setGyroY(double newGyroY);

        /**
         * Set current Z rotation to the given value
         * @param newGyroZ The new Z rotation to set
         */
        void setGyroZ(double newGyroZ);

        /**
         * Get the human-readable name for this device
         * @return The device name
         */
        std::string getDeviceName();

    protected:
        void applyDefaultState();
        std::vector<uint8_t> getCreateData();
        void handleData(const std::vector<uint8_t> &data);
    
    private:
        double gyroX = 0, gyroY = 0, gyroZ = 0, accelX = 0, accelY = 0, accelZ = 0;
        double gyroXOffset = 0, gyroYOffset = 0, gyroZOffset = 0;
    };


    /**
     * \class NxpAdafruit9Dof sensor.hpp arpirobot/arduino/sensor.hpp
     * 
     * Newer adafruit 9-DOF NXP IMU
     */
    class NxpAdafruit9Dof : public ArduinoDevice{
    public:
        /**
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        NxpAdafruit9Dof(bool createDevice = true, int deviceId = -1);

        /**
         * Calibrate the IMU. Should reduce gyro drift and accelerometer error
         * IMU MUST BE STATIONARY DURING CALIBRATION. 
         * GRAVITATIONAL ACCELERATION MUST BE IN NEGATIVE Z DIRECTION DURING CALIBRATION.
         * @param samples The number of samples to take during calibration. 
         *                More samples may be more accurate, but will take longer.
         */
        void calibrate(uint16_t samples);

        /**
         * Get the X rotation
         * @return X rotation in degrees
         */
        double getGyroX();

        /**
         * Get the Y rotation
         * @return Y rotation in degrees
         */
        double getGyroY();

        /**
         * Get the Z rotation
         * @return Z rotation in degrees
         */
        double getGyroZ();

        /**
         * Get the X axis acceleration
         * @return X acceleration in m/s^2
         */
        double getAccelX();

        /**
         * Get the Y axis acceleration
         * @return Y acceleration in m/s^2
         */
        double getAccelY();

        /**
         * Get the Z axis acceleration
         * @return Z acceleration in m/s^2
         */
        double getAccelZ();

        /**
         * Set current X rotation to the given value
         * @param newGyroX The new X rotation to set
         */
        void setGyroX(double newGyroX);

        /**
         * Set current Y rotation to the given value
         * @param newGyroY The new Y rotation to set
         */
        void setGyroY(double newGyroY);

        /**
         * Set current Z rotation to the given value
         * @param newGyroZ The new Z rotation to set
         */
        void setGyroZ(double newGyroZ);

        /**
         * Get the human-readable name for this device
         * @return The device name
         */
        std::string getDeviceName();

    protected:
        void applyDefaultState();
        std::vector<uint8_t> getCreateData();
        void handleData(const std::vector<uint8_t> &data);
    
    private:
        double gyroX = 0, gyroY = 0, gyroZ = 0, accelX = 0, accelY = 0, accelZ = 0;
        double gyroXOffset = 0, gyroYOffset = 0, gyroZOffset = 0;
    };

    /**
     * \class Mpu6050Imu sensor.hpp arpirobot/arduino/sensor.hpp
     * 
     * MPU6050 IMU
     * https://www.adafruit.com/product/1714
     */
    class Mpu6050Imu : public ArduinoDevice{
    public:

        /**
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        Mpu6050Imu(bool createDevice = true, int deviceId = -1);

        /**
         * Calibrate the IMU. Should reduce gyro drift and accelerometer error
         * IMU MUST BE STATIONARY DURING CALIBRATION. 
         * GRAVITATIONAL ACCELERATION MUST BE IN NEGATIVE Z DIRECTION DURING CALIBRATION.
         * @param samples The number of samples to take during calibration. 
         *                More samples may be more accurate, but will take longer.
         */
        void calibrate(uint16_t samples);

        /**
         * Get the X rotation
         * @return X rotation in degrees
         */
        double getGyroX();

        /**
         * Get the Y rotation
         * @return Y rotation in degrees
         */
        double getGyroY();

        /**
         * Get the Z rotation
         * @return Z rotation in degrees
         */
        double getGyroZ();

        /**
         * Get the X axis acceleration
         * @return X acceleration in m/s^2
         */
        double getAccelX();

        /**
         * Get the Y axis acceleration
         * @return Y acceleration in m/s^2
         */
        double getAccelY();

        /**
         * Get the Z axis acceleration
         * @return Z acceleration in m/s^2
         */
        double getAccelZ();

        /**
         * Set current X rotation to the given value
         * @param newGyroX The new X rotation to set
         */
        void setGyroX(double newGyroX);

        /**
         * Set current Y rotation to the given value
         * @param newGyroY The new Y rotation to set
         */
        void setGyroY(double newGyroY);

        /**
         * Set current Z rotation to the given value
         * @param newGyroZ The new Z rotation to set
         */
        void setGyroZ(double newGyroZ);

        /**
         * Get the human-readable name for this device
         * @return The device name
         */
        std::string getDeviceName();

    protected:
        void applyDefaultState();
        std::vector<uint8_t> getCreateData();
        void handleData(const std::vector<uint8_t> &data);
    
    private:
        double gyroX = 0, gyroY = 0, gyroZ = 0, accelX = 0, accelY = 0, accelZ = 0;
        double gyroXOffset = 0, gyroYOffset = 0, gyroZOffset = 0;
    };

}