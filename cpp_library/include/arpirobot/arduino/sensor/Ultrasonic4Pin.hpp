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
     * \class Ultrasonic4Pin Ultrasonic4Pin.hpp arpirobot/arduino/sensor/Ultrasonic4Pin.hpp
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
        std::string getDeviceName() override;
    
    protected:
        void applyDefaultState() override;
        std::vector<uint8_t> getCreateData() override;
        void handleData(const std::vector<uint8_t> &data) override;
    
    private:
        std::string triggerPin;
        std::string echoPin;
        int distance = 0;
    };
    
}
