#pragma once

#include <arpirobot/core/robot/BaseRobot.hpp>
#include <arpirobot/core/audio/AudioManager.hpp>
#include <arpirobot/devices/gamepad/Gamepad.hpp>
#include <arpirobot/devices/adafruitmotorhat/AdafruitMotorHatMotor.hpp>
#include <arpirobot/core/drive/ArcadeDriveHelper.hpp>
#include <arpirobot/arduino/iface/ArduinoUartInterface.hpp>
#include <arpirobot/arduino/sensor/VoltageMonitor.hpp>

using namespace arpirobot;


class Robot : public BaseRobot{
public:
    void robotStarted();

    void robotEnabled();

    void robotDisabled();

    void enabledPeriodic();

    void disabledPeriodic();

    void periodic();

private:
    Gamepad gp0 {0};
    AdafruitMotorHatMotor leftMotor {2};
    AdafruitMotorHatMotor rightMotor {3};
    ArcadeDriveHelper driveHelper {&leftMotor, &rightMotor};

    ArduinoUartInterface arduino{"/dev/ttyUSB0", 57600};
    VoltageMonitor vmon {"A0", 4.85, 30000, 7500};
};