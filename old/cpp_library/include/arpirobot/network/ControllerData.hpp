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

#include <mutex>
#include <vector>

namespace arpirobot{

    ////////////////////////////////////////////////////////////////////////////
    /// ControllerData
    ////////////////////////////////////////////////////////////////////////////

    /**
     * \class ControllerData ControllerData.hpp arpirobot/core/network/ControllerData.hpp
     * 
     * Class to hold and data for a single controller and parse data received from network
     */
    class ControllerData{
    public:

        /**
         * @param data The raw controller data from network
         */
        ControllerData(std::vector<uint8_t> &data);

        /**
         * Update the controller data from raw network data
         * @param data Raw controller data from network
         */
        void updateData(std::vector<uint8_t> &data);

        int controllerNumber = -1;
        int axisCount = -1;
        int buttonCount = -1;
        int dpadCount = -1;
        std::vector<float> axes;
        std::vector<bool> buttons;
        std::vector<int> dpads;
        std::vector<uint8_t> lastData;
        std::chrono::steady_clock::time_point lastUpdateTime;
        std::mutex lock;
    };

}