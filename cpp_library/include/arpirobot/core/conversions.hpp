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

#include <cstdint>
#include <vector>

namespace arpirobot{

    class Conversions{
    public:
        static void checkBigEndian();
    
        static std::vector<uint8_t> convertInt32ToData(int32_t input, bool littleEndian);

        static int32_t convertDataToInt32(const std::vector<uint8_t> &data, size_t offset, bool littleEndian);

        static std::vector<uint8_t> convertInt16ToData(int16_t input, bool littleEndian);

        static int16_t convertDataToInt16(const std::vector<uint8_t> &data, size_t offset, bool littleEndian);

        static std::vector<uint8_t> convertFloatToData(float input, bool littleEndian);

        static float convertDataToFloat(const std::vector<uint8_t> &data, size_t offset, bool littleEndian);

        static bool isBigEndian;
    };

}