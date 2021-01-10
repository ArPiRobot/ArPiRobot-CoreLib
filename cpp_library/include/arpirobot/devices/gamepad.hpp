#pragma once

#include <arpirobot/core/device.hpp>
#include <arpirobot/devices/gamepad.hpp>
#include <arpirobot/core/drive.hpp>

#include <unordered_map>

namespace arpirobot{
    class Gamepad : public BaseDevice{
    public:
        Gamepad(int controllerNum);

        bool isEnabled();

        bool shouldMatchRobotState();

        bool shouldDisableWithWatchdog();

        void _enable();

        void _disable();

        int getControllerNum();

        double getAxis(int axisNum, double deadband = 0);

        bool getButton(int buttonNum);

        int getDpad(int dpadNum);

        void setAxisTransform(int axisNum, BaseAxisTransform *transform);

        void clearAxisTransform(int axisNum);

        std::string getDeviceName();
    
    protected:
        void begin();

    private:
        int controllerNum;
        std::unordered_map<int, BaseAxisTransform*> axisTransforms;
    };
}