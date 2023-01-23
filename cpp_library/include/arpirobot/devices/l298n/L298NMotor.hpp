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

#include <string>
#include <arpirobot/core/device/MotorController.hpp>
#include <arpirobot/core/io/IoDevice.hpp>

namespace arpirobot{

    /**
     * \class L298NMotor L298NMotor.hpp arpirobot/devices/L298NMotor.hpp
     * 
     * Motor on L298N module
     */
    class L298NMotor : public MotorController, public IoDevice{
    public:

        /**
         * @param in1Pin pin for side 1 directional control
         * @param in2Pin pin for side 2 directional control
         * @param pwmPin pin for pwm of motor
         */
        L298NMotor(int in1Pin, int in2Pin, int pwmPin);

        L298NMotor(const L298NMotor &other) = delete;
        
        ~L298NMotor();

        L298NMotor &operator=(const L298NMotor &other) = delete;

    protected:
        void begin() override;
        
        void run() override;

        void close() override;

    private:
        int in1, in2, pwm;
    };
}