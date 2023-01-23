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
#include <arpirobot/core/network/MainVmon.hpp>

namespace arpirobot{
    /**
     * \class VoltageMonitor VoltageMonitor.hpp arpirobot/arduino/sensor/VoltageMonitor.hpp
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
    
    protected:
        void applyDefaultState() override;
        std::vector<uint8_t> getCreateData() override;
        void handleData(const std::vector<uint8_t> &data) override;
    
    private:
        std::string pin;
        double vboard;
        int r1;
        int r2;
        double voltage = 0;
    };

}
