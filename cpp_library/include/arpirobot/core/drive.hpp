#pragma once

#include <arpirobot/core/device.hpp>
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

    /**
     * Base class for axis transforms. Axis transforms take an analog axis value (-1.0 to 1.0)
     * and apply a fixed calculation to modify the curve of the axis.
     * 
     * Axis transforms are most useful with drive control.
     */
    class BaseAxisTransform{
    public:
        /**
         * Axis transforms should override this functino to implement specific calculations.
         */
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
        /**
         * @param minPower The minimum output of this cubic function (leave as zero if unsure)
         * @param midPower The "middle" power where the "flat" part of the function should be positioned
         */
        CubicAxisTransform(double minPower, double midPower);

        double applyTransform(double rawAxisValue);

    private:
        // a*(x^3) + b*(x^2) + c*x + d
        double a, b, c, d;

        static std::vector<double> polyfit(std::vector<double> x, std::vector<double> y, int order);
    };
}