#pragma once

#include <arpirobot/core/robot.hpp>
#include <arpirobot/devices/adafruitmotorhat.hpp>
#include <arpirobot/core/drive.hpp>
#include <arpirobot/devices/gamepad.hpp>
#include <arpirobot/arduino/iface.hpp>
#include <arpirobot/arduino/sensor.hpp>

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

    Gamepad gp0 {0};

    ArduinoUartInterface arduino {"/dev/ttyUSB0", 57600};
    VoltageMonitor vmon {"A0", 4.85, 30000, 7500};
    SingleEncoder enc {2, false};
};