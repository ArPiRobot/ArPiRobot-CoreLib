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

namespace arpirobot{

    /**
     * \class RobotProfile RobotProfile.hpp arpirobot/core/robot/RobotProfile.hpp
     * 
     * Settings to configure general robot behavior
     */
    class RobotProfile{
    public:
        /// Number of threads to use on the main scheduler
        static int mainSchedulerThreads; 

        /// Rate to run robot periodic functions at (ms)
        static int periodicFunctionRate; 

        /// Maximum age gamepad data can be before it is considered invalid (ms)
        static int maxGamepadDataAge;  
        
        /// Rate to run action periodic functions at (ms) 
        static int actionFunctionPeriod; 

        /// Name of the IO provider to use (empty string for default)
        static std::string ioProvider;
    };
}