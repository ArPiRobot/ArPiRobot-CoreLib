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

#include <arpirobot/devices/tb6612/TB6612Motor.hpp>

namespace arpirobot{
    /**
     * \class TB6612Module TB6612Module.hpp arpirobot/devices/TB6612Module.hpp
     * 
     * TB6612 Module (two motors)
     */
    class TB6612Module{
    public:

        /**
         * @param aIn1Pin In1 pin for motor a of module
         * @param aIn2Pin In2 pin for motor a of module
         * @param pwmaPin PWM pin for motor a of module
         * @param bIn1Pin In1 pin for motor b of module
         * @param bIn2Pin In2 pin for motor b of module
         * @param pwmbPin PWM pin for motor b of module
         */
        TB6612Module(int aIn1Pin, int aIn2Pin, int pwmaPin, int bIn1Pin, int bIn2Pin, int pwmbPin);

        /**
         * Get the Motor instance for motor a
         */
        TB6612Motor &getMotorA();

        /**
         * Get the Motor instance for motor b
         */
        TB6612Motor &getMotorB();

    private:
        TB6612Motor motorA, motorB;
    };
}