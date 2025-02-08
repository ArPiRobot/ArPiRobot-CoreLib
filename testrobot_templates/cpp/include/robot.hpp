#pragma once

#include <arpirobot/robot/BaseRobot.hpp>

#include <actions.hpp>


using namespace arpirobot;


class Robot : public BaseRobot{
public:
    void robotStarted();
    void robotStopped();
    void robotEnabled();
    void robotDisabled();
    void enabledPeriodic();
    void disabledPeriodic();
    void periodic();
};
