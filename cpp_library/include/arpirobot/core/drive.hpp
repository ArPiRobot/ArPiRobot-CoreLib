#pragma once

#include <arpirobot/core/device.hpp>
#include <vector>
#include <memory>
#include <array>

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

    class BaseAxisTransform{
    public:
        virtual double applyTransform(double rawAxisValue) = 0;
    };

    /**
     * Transform that applies a square root function to the magnitude of the input.
     * Sign of the output will match the sign of the input
     */
    class SquareRootAxisTransform : public BaseAxisTransform {
    public:
        double applyTransform(double rawAxisValue);
    };

    /**
     * Transform that applies a cubic function to the magnitude of the input.
     * The cubic function is fit to the data set (0, minPower), (0.5, midPower), (1, 1)
     * This gives a wide range of inputs that will yield a value near midPower
     * Sign of the output will match the sign of the input
     */
    class CubicAxisTransform : public BaseAxisTransform {
    public:
        CubicAxisTransform(double minPower, double midPower);

        double applyTransform(double rawAxisValue);

    private:
        // a*(x^3) + b*(x^2) + c*x + d
        double a, b, c, d;

        static std::vector<double> polyfit(std::vector<double> x, std::vector<double> y, int order);
    };
}