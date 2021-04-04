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
     * \class SingleEncoder SingleEncoder.hpp arpirobot/arduino/sensor/SingleEncoder.hpp
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

}
