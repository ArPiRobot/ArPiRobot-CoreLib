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
#include <mutex>
#include <arpirobot/core/device/BaseDevice.hpp>

namespace arpirobot{
    /**
     * \class MotorController MotorController.hpp arpirobot/core/device/MotorController.hpp
     * 
     * Base class for motor controllers.
     */
    class MotorController : public BaseDevice{
    public:

        virtual ~MotorController();

        /**
         * Check if the motor direction is inverted (positive and negative speed switched)
         * @return true if inverted, else false
         */
        bool isInverted();

        /**
         * Set the motor direction to inverted or non-inverted (positive and negative speed switched)
         * @param inverted True for inverted, false for non-inverted
         */
        void setInverted(bool inverted);

        /**
         * Check if brake mode is enabled. When enabled motors with a speed of zero will resist motion.
         * @return true if enabled, else false
         */
        bool isBrakeMode();

        /**
         * Set if brake mode is enabled
         * @param brakeMode true for enabled, false for disabled
         */
        void setBrakeMode(bool brakeMode);

        /**
         * Get the current speed of the motor
         * @return the motor's speed
         */
        double getSpeed();

        /**
         * Set the current speed of the motor (no effect if motor is disabled)
         * @param speed The motor's speed (between -1.0 and 1.0)
         */
        void setSpeed(double speed);

    protected:
        bool isEnabled();

        bool shouldMatchRobotState();

        bool shouldDisableWithWatchdog();

        void enable();

        void disable();
        
        virtual void run() = 0;

        double speed = 0;
        std::mutex lock;
        bool enabled = false;
        bool brakeMode = false;
        int8_t speedFactor = 1; // 1 or -1
    };

}