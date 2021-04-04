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

#include <arpirobot/devices/l298n/L298NMotor.hpp>

namespace arpirobot{
    /**
     * \class L298NModule L298NModule.hpp arpirobot/devices/L298NModule.hpp
     * 
     * L298N Module (two motors)
     */
    class L298NModule{
    public:

        /**
         * @param in1Pin In1 pin for module (motor a)
         * @param in2Pin In2 pin for module (motor a)
         * @param enaPin PWM pin for motor a of module
         * @param in3Pin In3 pin for module (motor b)
         * @param in4Pin In4 pin for module (motor b)
         * @param enbPin PWM pin for motor b of module
         */
        L298NModule(int in1Pin, int in2Pin, int enaPin, int in3Pin, int in4Pin, int enbPin);

        /**
         * Get the Motor instance for motor a
         */
        L298NMotor &getMotorA();

        /**
         * Get the Motor instance for motor b
         */
        L298NMotor &getMotorB();

    private:
        L298NMotor motorA, motorB;
    };
}