#pragma once

#include <arpirobot/core/device.hpp>

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

        // TODO: Axis transforms

        std::string getDeviceName();
    
    protected:
        void begin();

    private:
        int controllerNum;
    };
}