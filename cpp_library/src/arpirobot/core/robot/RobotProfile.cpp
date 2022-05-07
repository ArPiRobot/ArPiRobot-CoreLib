/*
 * Copyright 2022 Marcus Behel
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


#include <arpirobot/core/robot/RobotProfile.hpp>

using namespace arpirobot;


int RobotProfile::mainSchedulerThreads = 10;
int RobotProfile::periodicFunctionRate = 50;
int RobotProfile::maxGamepadDataAge = 100;
int RobotProfile::actionFunctionPeriod = 50;
std::string RobotProfile::ioProvider = "";
