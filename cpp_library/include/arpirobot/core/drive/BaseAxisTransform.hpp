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
#include <vector>
#include <memory>
#include <array>

namespace arpirobot{
    /**
     * \class BaseAxisTransform BaseAxisTransform.hpp arpirobot/core/drive/BaseAxisTransform.hpp
     * 
     * Base class for axis transforms. Axis transforms take an analog axis value (-1.0 to 1.0)
     * and apply a fixed calculation to modify the curve of the axis.
     * 
     * Axis transforms are most useful with drive control.
     */
    class BaseAxisTransform{
    public:
        virtual ~BaseAxisTransform() = default;

        /**
         * Axis transforms should override this function to implement specific calculations.
         */
        virtual double applyTransform(double rawAxisValue) = 0;
    };
}
