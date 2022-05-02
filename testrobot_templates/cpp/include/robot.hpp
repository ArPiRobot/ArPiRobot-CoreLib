#pragma once

#include <arpirobot/core/robot/BaseRobot.hpp>

#include <actions.hpp>

#include <arpirobot/arduino/iface/ArduinoUartInterface.hpp>
#include <arpirobot/arduino/sensor/Mpu6050Imu.hpp>
#include <arpirobot/devices/adafruitmotorhat/AdafruitMotorHatMotor.hpp>
#include <arpirobot/core/drive/ArcadeDriveHelper.hpp>

using namespace arpirobot;


class Robot : public BaseRobot{
public:
    void robotStarted();
    void robotEnabled();
    void robotDisabled();
    void enabledPeriodic();
    void disabledPeriodic();
    void periodic();

    AdafruitMotorHatMotor flmotor {3};
    AdafruitMotorHatMotor rlmotor {4};
    AdafruitMotorHatMotor frmotor {2};
    AdafruitMotorHatMotor rrmotor {1};

    ArcadeDriveHelper driveHelper {{&flmotor, &rlmotor}, {&frmotor, &rrmotor}};

    ArduinoUartInterface arduino {"/dev/ttyUSB0", 57600};
    Mpu6050Imu imu;
    TestAction myTestAction;
};
