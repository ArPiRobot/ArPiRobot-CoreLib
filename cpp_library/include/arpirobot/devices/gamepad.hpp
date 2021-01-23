#pragma once

#include <arpirobot/core/device.hpp>
#include <arpirobot/devices/gamepad.hpp>
#include <arpirobot/core/drive.hpp>
#include <arpirobot/core/action.hpp>

#include <unordered_map>

namespace arpirobot{

    /**
     * Gamepad receiving data from network table
     */
    class Gamepad : public BaseDevice{
    public:
        /**
         * @param controllerNum The controller number for this controller. Top controller in DS is zero
         */
        Gamepad(int controllerNum);

        /**
         * Get the controller number for this gamepad
         * @returns The controller number
         */
        int getControllerNum();

        /**
         * Get an axis for this controller
         * @param axisNum The axis number
         * @param deadband A minimum threshold for axis values. Values below this will be returned as zero.
         * @returns The axis value after applying the deadband and (if required) an axis transform
         */
        double getAxis(int axisNum, double deadband = 0);

        /**
         * Get a button for the controller
         * @param buttonNum The button number
         * @returns true if pressed, else false
         */
        bool getButton(int buttonNum);

        /**
         * Get the value of a dpad
         * @param dpadNum The dpad number
         * @returns 0 if center, 1 for up through 8 going clockwise
         */
        int getDpad(int dpadNum);

        /**
         * Set the axis transform for a given axis
         * @param axisNum The axis number to apply a transform to
         * @param transform The transform
         */
        void setAxisTransform(int axisNum, BaseAxisTransform *transform);

        /**
         * Remove a transform from an axis
         * @param axisNum The axis number to clear a transform from
         */
        void clearAxisTransform(int axisNum);

        /**
         * Get the human-readable name of the device
         * @returns the devices human readable name
         */
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

    /**
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

    /**
     * Action trigger for when a button on a gamepad is released
     */
    class ButtonReleasedTrigger : public BaseActionTrigger {
    public:
        /**
         * @param gamepad The gamepad to check for button presses on
         * @param buttonNum The button number
         * @param targetAction The action to run when this occurs
         * @param doRestart If true this will restart the action if it is running when this occurs
         */
        ButtonReleasedTrigger(Gamepad *gamepad, int buttonNum, Action *targetAction, bool doRestart = true);
        bool shouldRun();
    
    private:
        Gamepad *gamepad;
        int buttonNum;
        bool lastValue = false;
    };

}