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

#include <arpirobot/arduino/iface/BaseArduinoInterface.hpp>

#include <string>
#include <vector>

namespace arpirobot{

    /**
     * \class ArduinoDevice ArduinoDevice.hpp arpirobot/arduino/device/ArduinoDevice.hpp
     * 
     * Common base class for any device that is connected to an arduino (any device that communicates with the Pi via 
     * an arduino or other microprocessor)
     */
    class ArduinoDevice{
    public:

        /**
         * @param createDevice If this is true the arduino this device is added to will be instructed to instantiate
         *                     this device. This should only be false if a device is hard-coded in the arduino firmware
         * @param deviceId Assign a deviceId for a device that is hard-coded in the arduino firmware
         */
        ArduinoDevice(bool createDevice, int deviceId);
    
        virtual ~ArduinoDevice();

        /**
         * Get the human-readable name for this device
         */
        virtual std::string getDeviceName() = 0;

    protected:
        static std::vector<uint8_t> stringToData(const std::string &str);

        void setArduino(BaseArduinoInterface *arduino);
        void setDeviceId(int deviceId);

        bool sendData(const std::vector<uint8_t> &data);
        bool sendData(const std::string &data);

        // Device specific
        virtual void applyDefaultState() = 0;
        virtual std::vector<uint8_t> getCreateData() = 0;
        virtual void handleData(const std::vector<uint8_t> &data) = 0;

        bool createDevice;
        int deviceId;
        BaseArduinoInterface *arduino = nullptr;

        friend class BaseArduinoInterface;
    };
}