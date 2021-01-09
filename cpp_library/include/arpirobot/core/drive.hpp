#pragma once

#include <arpirobot/core/device.hpp>
#include <vector>
#include <memory>

namespace arpirobot{
    class ArcadeDriveHelper{
    public:
        ArcadeDriveHelper(MotorController *leftMotor, MotorController *rightMotor);
        ArcadeDriveHelper(std::vector<MotorController*> leftMotors, std::vector<MotorController*> rightMotors);

        void updateSpeed(double newSpeed);
        void updateRotation(double newRotation);
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

    class TankDriveHelper{
    public:
        TankDriveHelper(MotorController *leftMotor, MotorController *rightMotor);
        TankDriveHelper(std::vector<MotorController*> leftMotors, std::vector<MotorController*> rightMotors);

        void updateLeftSpeed(double newLeftSpeed);
        void updateRightSpeed(double newRightSpeed);
        void update(double newLeftSpeed, double newRightSpeed);
    
    private:
        void doUpdate();

        std::vector<MotorController*> leftMotors;
        std::vector<MotorController*> rightMotors;
        double leftSpeed = 0;
        double rightSpeed = 0;
    };
}