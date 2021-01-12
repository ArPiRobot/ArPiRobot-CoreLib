#pragma once

#include <arpirobot/core/robot.hpp>
#include <arpirobot/devices/ina260.hpp>
#include <arpirobot/devices/gamepad.hpp>

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

    INA260PowerSensor vmon;
};