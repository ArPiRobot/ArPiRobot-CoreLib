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

namespace arpirobot{

    /**
     * \class MainVmon MainVmon.hpp arpirobot/core/network/MainVmon.hpp
     * 
     * Main voltage monitor interface
     * Any sensor (arduino or pi device) can implement this interface to be able to be used as a main vmon
     */
    class MainVmon{
    public:

        /**
         * Make this device the main voltage monitor
         */
        void makeMainVmon();

    protected:
        /**
         * Returns true if this is currently the main vmon
         */
        bool isMainVmon();

        /**
         * Send the main battery voltage to the connected DS (if this is the main vmon)
         */
        void sendMainBatteryVoltage(double voltage);
    };

}