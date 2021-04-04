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

package arpirobot.core.robot;

/**
 * Settings to configure general robot behavior
 */
public class RobotProfile {
    /// Number of threads to use on the main scheduler
    public int mainSchedulerThreads = 10;

    /// Rate to run robot periodic functions at (ms)
    public int periodicFunctionRate = 50;

    /// Maximum age gamepad data can be before it is considered invalid (ms)
    public int maxGamepadDataAge = 100;

    /// Rate to run action periodic functions at (ms) 
    public int actionFunctionPeriod = 50;
}
