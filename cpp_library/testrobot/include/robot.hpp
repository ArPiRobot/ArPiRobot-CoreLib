#pragma once

#include <arpirobot/core/robot.hpp>
#include <arpirobot/devices/drv8833.hpp>
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

    Gamepad gp0 {0};

    DRV8833Module drv8833 {24, 25, 17, 27, 23};
    DRV8833Motor &motor1 {drv8833.getMotorA()};
    DRV8833Motor &motor2 {drv8833.getMotorB()};
};