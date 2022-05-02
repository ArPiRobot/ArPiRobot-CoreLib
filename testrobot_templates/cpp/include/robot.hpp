#pragma once

#include <arpirobot/core/robot/BaseRobot.hpp>

#include <actions.hpp>


using namespace arpirobot;


class Robot : public BaseRobot{
public:
    void robotStarted();
    void robotEnabled();
    void robotDisabled();
    void enabledPeriodic();
    void disabledPeriodic();
    void periodic();
};
