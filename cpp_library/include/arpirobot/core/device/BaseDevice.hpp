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

#include <string>
#include <mutex>
#include <memory>

namespace arpirobot{

    class Action; // Forward declare b/c this header is included by action.hpp

    /**
     * \class BaseDevice BaseDevice.hpp arpirobot/core/device/BaseDevice.hpp
     * 
     * Base class for any device connected to the raspberry pi.
     */
    class BaseDevice{
    public:

        virtual ~BaseDevice();

        /**
         * Get the human-readable name of the device
         * @return the devices human readable name
         */
        virtual std::string getDeviceName() = 0;

        /**
         * @brief Check if a given action locks the device.
         * 
         * @param action The action to compare to. If nullptr this function 
         *               checks if any action locks the device
         * 
         * @return true If the device is locked by the action
         * @return false If the device is not locked the action
         */
        bool isLockedByAction(std::shared_ptr<Action> action = nullptr);

        /**
         * @brief Check if a given action locks the device.
         * 
         * @param action The action to compare to.
         * 
         * @return true If the device is locked by the action
         * @return false If the device is not locked the action
         */
        bool isLockedByAction(Action &action);

    protected:
        virtual void begin() = 0;
        virtual bool isEnabled() = 0;
        virtual bool shouldMatchRobotState() = 0;
        virtual bool shouldDisableWithWatchdog() = 0;
        virtual void enable() = 0;
        virtual void disable() = 0;
    
        bool initialized = false;

    private:

        void lockDevice(Action *action);

        void releaseDevice(Action *action);

        void doBegin();

        std::mutex actionLock;

        // Not shared_ptr because should not keep Action in scope
        // Action will set when locking
        // Action will unset on unlock or on finish
        Action *lockingAction = nullptr;

        friend class Action; // Needs to call lockDevice
        friend class BaseRobot; // Needs doBegin, enable, disable
    };
}