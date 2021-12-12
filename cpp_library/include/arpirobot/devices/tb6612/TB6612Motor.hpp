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
     * \class TB6612Motor TB6612Motor.hpp arpirobot/devices/TB6612Motor.hpp
     * 
     * Motor on TB6612 module
     */
    class TB6612Motor : public MotorController, public IoDevice{
    public:
        /**
         * @param in1Pin pin for side 1 directional control
         * @param in2Pin pin for side 2 directional control
         * @param pwmPin pin for pwm of motor
         */
        TB6612Motor(int in1Pin, int in2Pin, int pwmPin);

        TB6612Motor(const TB6612Motor &other) = delete;
        
        ~TB6612Motor();

        TB6612Motor &operator=(const TB6612Motor &other) = delete;

        /**
         * Get the human-readable name of the device
         * @return the devices human readable name
         */
        std::string getDeviceName() override;

    protected:
        void begin() override;
        
        void run() override;

        void close() override;

    private:
        int in1, in2, pwm;
    };
}