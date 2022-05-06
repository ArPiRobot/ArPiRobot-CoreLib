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
     * \class ArcadeDriveHelper ArcadeDriveHelper.hpp arpirobot/core/drive/ArcadeDriveHelper.hpp
     * 
     * Drive helper for arcade drive method.
     * 
     * Drive helpers calculate speeds for each motor based on a set of inputs and update all motors speeds at once.
     * Arcade drive uses two inputs: speed and rotation.
     * 
     */
    class ArcadeDriveHelper{
    public:
        /**
         * @param leftMotor A singular left side motor (reference, must remain in scope)
         * @param rightMotor A singular right side motor (reference, must remain in scope)
         */
        ArcadeDriveHelper(MotorController &leftMotor, MotorController &rightMotor);

        /**
         * @param leftMotor A singular left side motor (shared_ptr)
         * @param rightMotor A singular right side motor (shared_ptr)
         */
        ArcadeDriveHelper(std::shared_ptr<MotorController> leftMotor, std::shared_ptr<MotorController> rightMotor);

        /**
         * @param leftMotors A set of left motors (references, must remain in scope)
         * @param rightMotors A set of right motors (references, must remain in scope)
         */
        ArcadeDriveHelper(std::vector<std::reference_wrapper<MotorController>> leftMotors, std::vector<std::reference_wrapper<MotorController>> rightMotors);

        /**
         * @param leftMotors A set of left motors (shared_ptrs)
         * @param rightMotors A set of right motors (shared_ptrs)
         */
        ArcadeDriveHelper(std::vector<std::shared_ptr<MotorController>> leftMotors, std::vector<std::shared_ptr<MotorController>> rightMotors);

        /**
         * Set a new speed value. Speeds will be re-calculated and updated for each motor
         * @param newSpeed the new speed (-1.0 to 1.0)
         */
        void updateSpeed(double newSpeed);

        /**
         * Set a new rotation value. Speeds will be re-calculated and updated for each motor
         * @param newRotation the new rotation (-1.0 to 1.0)
         */
        void updateRotation(double newRotation);

        /**
         * Set a new speed and rotation value at the same time. 
         * Speeds will be re-calculated and updated for each motor.
         * If updating both values use this instead of calling updateSpeed and updateRotation
         * because this will only recalculate motor speeds once.
         * @param newSpeed the new speed (-1.0 to 1.0)
         * @param newRotation the new rotation (-1.0 to 1.0)
         */
        void update(double newSpeed, double newRotation);

    private:

        // Returns {leftSpeed, rightSpeed}
        std::array<double, 2> calculateSpeeds(double speed, double rotation);

        void doUpdate();

        std::vector<std::shared_ptr<MotorController>> leftMotors;
        std::vector<std::shared_ptr<MotorController>> rightMotors;
        double speed = 0;
        double rotation = 0;
    };
}
