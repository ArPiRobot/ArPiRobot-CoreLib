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

#include <arpirobot/core/device/MotorController.hpp>
#include <arpirobot/core/io/IoDevice.hpp>

namespace arpirobot{

    /**
     * \class DRV8833Motor DRV8833Motor.hpp arpirobot/devices/drv8833/DRV8833Motor.hpp
     * 
     * Motor on DRV8833 module
     */
    class DRV8833Motor : public MotorController, public IoDevice{
    public:

        /**
         * @param in1Pin pin for side 1 directional control
         * @param in2Pin pin for side 2 directional control
         * @param slpPin pin for sleep of module
         */
        DRV8833Motor(int in1Pin, int in2Pin, int slpPin);

        ~DRV8833Motor();

        /**
         * Get the human-readable name of the device
         * @return the devices human readable name
         */
        std::string getDeviceName();

    protected:
        void begin();
        
        void run();

        void close();

    private:
        int in1, in2, slp;
    };
}
