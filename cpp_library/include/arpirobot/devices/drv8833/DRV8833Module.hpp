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

#include <arpirobot/devices/drv8833/DRV8833Motor.hpp>

namespace arpirobot{
    /**
     * \class DRV8833Module DRV8833Module.hpp arpirobot/devices/drv8833/DRV8833Module.hpp
     * 
     * DRV8833 Module (two motors)
     */
    class DRV8833Module{
    public:

        /**
         * @param aIn1Pin In1 pin for motor a of module
         * @param aIn2Pin In2 pin for motor a of module
         * @param bIn1Pin In1 pin for motor b of module
         * @param bIn2Pin In2 pin for motor b of module
         * @param slpPin Sleep pin for module
         */
        DRV8833Module(int aIn1Pin, int aIn2Pin, int bIn1Pin, int bIn2Pin, int slpPin);

        /**
         * Get the Motor instance for motor a
         */
        DRV8833Motor &getMotorA();

        /**
         * Get the Motor instance for motor b
         */
        DRV8833Motor &getMotorB();

    private:
        DRV8833Motor motorA, motorB;
    };
    
}
