#pragma once

#include <arpirobot/devices/gamepad/Gamepad.hpp>
#include <arpirobot/core/action/BaseActionTrigger.hpp>


namespace arpirobot{
    /**
     * \class ButtonPressedTrigger ButtonPressedTrigger.hpp arpirobot/devices/gamepad/ButtonPressedTrigger.hpp
     * Action trigger for when a button on a gamepad is pressed
     */
    class ButtonPressedTrigger : public BaseActionTrigger{
    public:
        /**
         * @param gamepad The gamepad to check for button presses on
         * @param buttonNum The button number
         * @param targetAction The action to run when this occurs
         * @param doRestart If true this will restart the action if it is running when this occurs
         */
        ButtonPressedTrigger(Gamepad *gamepad, int buttonNum, Action *targetAction, bool doRestart = true);

        bool shouldRun();
    
    private:
        Gamepad *gamepad;
        int buttonNum;
        bool lastValue = false;
    };

}