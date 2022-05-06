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
     * \class TankDriveHelper TankDriveHelper.hpp arpirobot/core/drive/TankDriveHelper.hpp
     * 
     * Drive helper for tank drive method.
     * 
     * Drive helpers calculate speeds for each motor based on a set of inputs and update all motors speeds at once.
     * Tank drive uses two inputs: left speed and right speed.
     * 
     */
    class TankDriveHelper{
    public:
        /**
         * @param leftMotor A singular left side motor (reference, must remain in scope)
         * @param rightMotor A singular right side motor (reference, must remain in scope)
         */
        TankDriveHelper(MotorController &leftMotor, MotorController &rightMotor);

        /**
         * @param leftMotor A singular left side motor (shared_ptr)
         * @param rightMotor A singular right side motor (shared_ptr)
         */
        TankDriveHelper(std::shared_ptr<MotorController> leftMotor, std::shared_ptr<MotorController> rightMotor);

        /**
         * @param leftMotors A set of left motors (references, must remain in scope)
         * @param rightMotors A set of right motors (references, must remain in scope)
         */
        TankDriveHelper(std::vector<std::reference_wrapper<MotorController>> leftMotors, std::vector<std::reference_wrapper<MotorController>> rightMotors);

        /**
         * @param leftMotors A set of left motors (shared_ptrs)
         * @param rightMotors A set of right motors (shared_ptrs)
         */
        TankDriveHelper(std::vector<std::shared_ptr<MotorController>> leftMotors, std::vector<std::shared_ptr<MotorController>> rightMotors);

        /**
         * Set a new left speed value. Speeds will be re-calculated and updated for each motor
         * @param newLeftSpeed the new rotation (-1.0 to 1.0)
         */
        void updateLeftSpeed(double newLeftSpeed);

        /**
         * Set a new right speed value. Speeds will be re-calculated and updated for each motor
         * @param newRightSpeed the new rotation (-1.0 to 1.0)
         */
        void updateRightSpeed(double newRightSpeed);

        /**
         * Set a new left and right speed value at the same time. 
         * Speeds will be re-calculated and updated for each motor.
         * If updating both values use this instead of calling updateLeftSpeed and updateRightSpeed
         * because this will only recalculate motor speeds once.
         * @param newLeftSpeed the new speed (-1.0 to 1.0)
         * @param newRightSpeed the new rotation (-1.0 to 1.0)
         */
        void update(double newLeftSpeed, double newRightSpeed);
    
    private:
        void doUpdate();

        std::vector<std::shared_ptr<MotorController>> leftMotors;
        std::vector<std::shared_ptr<MotorController>> rightMotors;
        double leftSpeed = 0;
        double rightSpeed = 0;
    };
}
