#pragma once

#include <arpirobot/core/robot/BaseRobot.hpp>
#include <arpirobot/core/drive/ArcadeDriveHelper.hpp>
#include <arpirobot/devices/tb6612/TB6612Motor.hpp>
#include <arpirobot/devices/gamepad/Gamepad.hpp>

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

    TB6612Motor motor {1, 2, 3};

};