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
     * \class IRReflectorModule IRReflectorModule.hpp arpirobot/arduino/sensor/IRReflectorModule.hpp
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
}