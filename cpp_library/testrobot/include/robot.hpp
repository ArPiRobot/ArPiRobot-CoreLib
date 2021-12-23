#pragma once

#include <arpirobot/core/robot/BaseRobot.hpp>

#include <arpirobot/arduino/iface/ArduinoUartInterface.hpp>
#include <arpirobot/arduino/sensor/VoltageMonitor.hpp>
#include <arpirobot/arduino/sensor/SingleEncoder.hpp>

using namespace arpirobot;


class Robot : public BaseRobot{
public:
    void robotStarted();

    void robotEnabled();

    void robotDisabled();

    void enabledPeriodic();

    void disabledPeriodic();

    void periodic();

    VoltageMonitor vmon { "A0", 5.00, 30000, 7500 };
    SingleEncoder lencoder { 2, false };
    SingleEncoder rencoder { 3, false };
    ArduinoUartInterface arduino { "COM4", 57600 };
};