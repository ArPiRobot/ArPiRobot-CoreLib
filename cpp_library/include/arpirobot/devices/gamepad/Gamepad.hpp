#pragma once

#include <arpirobot/core/device/BaseDevice.hpp>
#include <arpirobot/core/drive/BaseAxisTransform.hpp>
#include <arpirobot/core/action/BaseActionTrigger.hpp>

#include <unordered_map>

namespace arpirobot{

    /**
     * \class Gamepad Gamepad.hpp arpirobot/devices/gamepad/Gamepad.hpp
     * Gamepad receiving data from drive station
     */
    class Gamepad : public BaseDevice{
    public:
        /**
         * @param controllerNum The controller number for this controller. Top controller in DS is zero
         */
        Gamepad(int controllerNum);

        /**
         * Get the controller number for this gamepad
         * @return The controller number
         */
        int getControllerNum();

        /**
         * Get an axis for this controller
         * @param axisNum The axis number
         * @param deadband A minimum threshold for axis values. Values below this will be returned as zero.
         * @return The axis value after applying the deadband and (if required) an axis transform
         */
        double getAxis(int axisNum, double deadband = 0);

        /**
         * Get a button for the controller
         * @param buttonNum The button number
         * @return true if pressed, else false
         */
        bool getButton(int buttonNum);

        /**
         * Get the value of a dpad
         * @param dpadNum The dpad number
         * @return 0 if center, 1 for up through 8 going clockwise
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
         * @return the devices human readable name
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

}