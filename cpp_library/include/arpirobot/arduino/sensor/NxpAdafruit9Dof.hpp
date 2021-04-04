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

#pragma once

#include <arpirobot/arduino/device/ArduinoDevice.hpp>
#include <arpirobot/arduino/iface/BaseArduinoInterface.hpp>

namespace arpirobot{
    /**
     * \class NxpAdafruit9Dof NxpAdafruit9Dof.hpp arpirobot/arduino/sensor/NxpAdafruit9Dof.hpp
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

}
