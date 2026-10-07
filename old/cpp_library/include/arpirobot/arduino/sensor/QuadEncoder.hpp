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
     * \class QuadEncoder QuadEncoder.hpp arpirobot/arduino/sensor/QuadEncoder.hpp
     * 
     * Quadrature encoder encoder (two channels)
     */
    class QuadEncoder : public ArduinoDevice{
    public:

        /**
         * @param pinA The digital pin number channel A is connected to
         * @param pinB The digital pin number channel B is connected to
         * @param useInternalPullup Set to true to use the arduino's internal pullup resistor on this pin (if supported)
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        QuadEncoder(int pinA, int pinB, bool useInternalPullup, bool createDevice = true, int deviceId = -1);

        /**
         * @param pinA The pin number channel A is connected to (prefix with letter A for analog pin)
         * @param pinB The digital pin number channel B is connected to
         * @param useInternalPullup Set to true to use the arduino's internal pullup resistor on this pin (if supported)
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        QuadEncoder(std::string pinA, int pinB, bool useInternalPullup, bool createDevice = true, int deviceId = -1);

        /**
         * @param pinA The digital pin number channel A is connected to
         * @param pinB The pin number channel B is connected to (prefix with letter A for analog pin)
         * @param useInternalPullup Set to true to use the arduino's internal pullup resistor on this pin (if supported)
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        QuadEncoder(int pinA, std::string pinB, bool useInternalPullup, bool createDevice = true, int deviceId = -1);

        /**
         * @param pinA The pin number channel A is connected to (prefix with letter A for analog pin)
         * @param pinB The pin number channel B is connected to (prefix with letter A for analog pin)
         * @param useInternalPullup Set to true to use the arduino's internal pullup resistor on this pin (if supported)
         * @param createDevice Leave this true unless the device is hard-coded in arduino firmware
         * @param deviceId Set this to the hard-coded deviceId if createDevice is false
         */
        QuadEncoder(std::string pinA, std::string pinB, bool useInternalPullup, bool createDevice = true, int deviceId = -1);

        /**
         * Get the position (tick count) for this encoder
         * @return The position in ticks
         */
        int32_t getPosition();

        /**
         * Set the current tick count to the given value
         * @param currentPosition The new tick value to set
         */
        void setPosition(int32_t currentPosition);

        /**
         * Get the current velocity from the encoder
         * @return The speed in ticks / sec
         */
        float getVelocity();
    
    protected:
        void applyDefaultState() override;
        std::vector<uint8_t> getCreateData() override;
        void handleData(const std::vector<uint8_t> &data) override;
    
    private:
        static const uint8_t numSamples = 5;

        std::string pinA, pinB;
        uint8_t useInternalPullup;
        int32_t count = 0;
        int32_t countOffset = 0;
        float velocity = 0;
    };
}
