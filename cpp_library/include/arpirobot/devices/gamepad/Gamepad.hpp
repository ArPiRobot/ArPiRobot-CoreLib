/*
 * Copyright 2021 Marcus Behel
 *
 * This file is part of ArPiRobot-CoreLib.
 * 
 * ArPiRobot-CoreLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ArPiRobot-CoreLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with ArPiRobot-CoreLib.  If not, see <https://www.gnu.org/licenses/>. 
 */

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

        Gamepad(const Gamepad &other) = delete;
        Gamepad &operator=(const Gamepad &other) = delete;

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
        std::string getDeviceName() override;
    
    protected:
        void begin() override;

        bool isEnabled() override;

        bool shouldMatchRobotState() override;

        bool shouldDisableWithWatchdog() override;

        void enable() override;

        void disable() override;

    private:
        int controllerNum;
        std::unordered_map<int, BaseAxisTransform*> axisTransforms;
    };

}