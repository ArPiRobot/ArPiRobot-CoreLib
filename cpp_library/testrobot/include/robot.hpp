#pragma once

#include <arpirobot/core/robot/BaseRobot.hpp>

#include <actions.hpp>

#include <arpirobot/arduino/iface/ArduinoUartInterface.hpp>
#include <arpirobot/arduino/sensor/QuadEncoder.hpp>


using namespace arpirobot;


class Robot : public BaseRobot{
public:
    void robotStarted();
    void robotEnabled();
    void robotDisabled();
    void enabledPeriodic();
    void disabledPeriodic();
    void periodic();

    ArduinoUartInterface arduino {"COM11", 57600};

    QuadEncoder enc {2, 3, true};
};
