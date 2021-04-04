#pragma once

#include <arpirobot/core/device/MotorController.hpp>
#include <vector>
#include <memory>
#include <array>

namespace arpirobot{
    /**
     * \class TankDriveHelper drive.hpp arpirobot/core/drive.hpp
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
         * @param leftMotor A singular left side motor
         * @param rightMotor A singular right side motor
         */
        TankDriveHelper(MotorController *leftMotor, MotorController *rightMotor);

        /**
         * @param leftMotors A set of left motors
         * @param rightMotors A set of right motors
         */
        TankDriveHelper(std::vector<MotorController*> leftMotors, std::vector<MotorController*> rightMotors);

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

        std::vector<MotorController*> leftMotors;
        std::vector<MotorController*> rightMotors;
        double leftSpeed = 0;
        double rightSpeed = 0;
    };
}