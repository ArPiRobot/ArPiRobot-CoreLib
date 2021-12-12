#pragma once

#include <arpirobot/core/robot/BaseRobot.hpp>
#include <arpirobot/core/audio/AudioManager.hpp>
#include <arpirobot/devices/gamepad/Gamepad.hpp>
#include <arpirobot/devices/adafruitmotorhat/AdafruitMotorHatMotor.hpp>
#include <arpirobot/core/drive/ArcadeDriveHelper.hpp>
#include <arpirobot/arduino/iface/ArduinoUartInterface.hpp>
#include <arpirobot/arduino/sensor/VoltageMonitor.hpp>
#include <arpirobot/devices/led/StatusLED.hpp>

using namespace arpirobot;


class Robot : public BaseRobot{
public:
    void robotStarted();

    void robotEnabled();

    void robotDisabled();

    void enabledPeriodic();

    void disabledPeriodic();

    void periodic();

private:
    StatusLED led {12};
};