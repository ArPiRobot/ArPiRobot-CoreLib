#pragma once

#include <arpirobot/core/device.hpp>
#include <arpirobot/devices/gamepad.hpp>
#include <arpirobot/core/drive.hpp>
#include <arpirobot/core/action.hpp>

#include <unordered_map>

namespace arpirobot{
    class Gamepad : public BaseDevice{
    public:
        Gamepad(int controllerNum);

        int getControllerNum();

        double getAxis(int axisNum, double deadband = 0);

        bool getButton(int buttonNum);

        int getDpad(int dpadNum);

        void setAxisTransform(int axisNum, BaseAxisTransform *transform);

        void clearAxisTransform(int axisNum);

        std::string getDeviceName();
    
    protected:
        void begin();

        bool isEnabled();

        bool shouldMatchRobotState();

        bool shouldDisableWithWatchdog();

        void enable();

        void disable();

    private:
        int controllerNum;
        std::unordered_map<int, BaseAxisTransform*> axisTransforms;
    };


    // Gamepad triggers for actions

    class ButtonPressedTrigger : public BaseActionTrigger{
    public:
        ButtonPressedTrigger(Gamepad *gamepad, int buttonNum, Action *targetAction, bool doRestart = true);

        bool shouldRun();
    
    private:
        Gamepad *gamepad;
        int buttonNum;
        bool lastValue = false;
    };

    class ButtonReleasedTrigger : public BaseActionTrigger {
    public:
        ButtonReleasedTrigger(Gamepad *gamepad, int buttonNum, Action *targetAction, bool doRestart = true);
        bool shouldRun();
    
    private:
        Gamepad *gamepad;
        int buttonNum;
        bool lastValue = false;
    };

}