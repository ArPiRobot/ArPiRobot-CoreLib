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
#include <arpirobot/core/drive/BaseAxisTransform.hpp>
#include <vector>
#include <memory>
#include <array>

namespace arpirobot{
    /**
     * \class CubicAxisTransform CubicAxisTransform.hpp arpirobot/core/drive/CubicAxisTransform.hpp
     * 
     * Transform that applies a cubic function to the magnitude of the input.
     * The cubic function is fit to the data set (0, minPower), (0.5, midPower), (1, 1)
     * This gives a wide range of inputs that will yield a value near midPower
     * Sign of the output will match the sign of the input
     * 
     * WARNING: Since input of 0 results in an output of minPower DO NOT use with a non-zero minPower
     * and a non-zero deadband.
     * 
     */
    class CubicAxisTransform : public BaseAxisTransform {
    public:
        /**
         * @param minPower The minimum output of this cubic function (leave as zero if unsure)
         * @param midPower The "middle" power where the "flat" part of the function should be positioned
         */
        CubicAxisTransform(double minPower, double midPower);

        double applyTransform(double rawAxisValue) override;

    private:
        // a*(x^3) + b*(x^2) + c*x + d
        double a, b, c, d;

        static std::vector<double> polyfit(std::vector<double> x, std::vector<double> y, int order);
    };
}
