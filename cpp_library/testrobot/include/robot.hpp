#pragma once

#include <arpirobot/core/robot.hpp>
#include <arpirobot/devices/l298n.hpp>
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

    L298NModule l298n {24, 25, 23, 17, 27, 22};
    L298NMotor &motor1 {l298n.getMotorA()};
    L298NMotor &motor2 {l298n.getMotorB()};
};