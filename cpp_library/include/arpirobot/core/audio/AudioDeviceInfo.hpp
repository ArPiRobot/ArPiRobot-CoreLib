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
#include <cstdint>

namespace arpirobot{
    /**
     * \class AudioDeviceInfo AudioDeviceInfo.hpp arpirobot/core/audio/AudioDeviceInfo.hpp
     * 
     * Class containing information about a specific audio device
     */
    class AudioDeviceInfo{
    public:
        static const uint8_t TYPE_PLAYBACK = 0;
        static const uint8_t TYPE_CAPTURE = 1;

        uint32_t id;
        std::string name;
        bool isDefault;
        uint8_t type;
    };
}