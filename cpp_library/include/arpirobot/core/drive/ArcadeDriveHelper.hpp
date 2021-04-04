#pragma once

#include <arpirobot/core/device/MotorController.hpp>
#include <vector>
#include <memory>
#include <array>

namespace arpirobot{
    /**
     * \class ArcadeDriveHelper drive.hpp arpirobot/core/drive.hpp
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
         * @param leftMotor A singular left side motor
         * @param rightMotor A singular right side motor
         */
        ArcadeDriveHelper(MotorController *leftMotor, MotorController *rightMotor);

        /**
         * @param leftMotors A set of left motors
         * @param rightMotors A set of right motors
         */
        ArcadeDriveHelper(std::vector<MotorController*> leftMotors, std::vector<MotorController*> rightMotors);

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

        std::vector<MotorController*> leftMotors;
        std::vector<MotorController*> rightMotors;
        double speed = 0;
        double rotation = 0;
    };
}
