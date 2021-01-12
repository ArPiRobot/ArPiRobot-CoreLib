#pragma once

#include <arpirobot/core/robot.hpp>
#include <arpirobot/devices/tb6612.hpp>
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

    TB6612Module tb6612 {24, 25, 23, 17, 27, 22};
    TB6612Motor &motor1 {tb6612.getMotorA()};
    TB6612Motor &motor2 {tb6612.getMotorB()};
};