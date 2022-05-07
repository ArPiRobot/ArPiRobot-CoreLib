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
#include <arpirobot/core/scheduler.hpp>

#include <vector>
#include <mutex>
#include <memory>
#include <functional>

namespace arpirobot{

    /**
     * \class Action Action.hpp arpirobot/core/action/Action.hpp
     * 
     * Generic action class. User actions should inherit this class and implement the four pure virtual methods
     */
    class Action{
    public:

        /**
         * Construct an Action
         * 
         * @param processRateMs Period between calls of the process function. 
         *                      Leave as -1 to use the value from the active robot profile
         */
        Action(int32_t processRateMs = -1);

        virtual ~Action();

        /**
         * Use this action to lock a set of devices. 
         * This is the same as calling Action::lockDevice once for each device individually.
         * @param devices A vector of devices to lock (references to)
         */
        void lockDevices(std::vector<std::reference_wrapper<BaseDevice>> devices);

        /**
         * Use this action to lock a set of devices. 
         * This is the same as calling Action::lockDevice once for each device individually.
         * @param devices A vector of devices to lock (shared_ptr to)
         */
        void lockDevices(std::vector<std::shared_ptr<BaseDevice>> devices);

        /**
         * Use this action to lock a device. When a device is locked by this action, 
         * any action previously locking it will be stopped.
         * @param device The device to lock (reference to)
         */
        void lockDevice(BaseDevice &device);

        /**
         * Use this action to lock a device. When a device is locked by this action, 
         * any action previously locking it will be stopped.
         * @param device The device to lock (shared_ptr to)
         */
        void lockDevice(std::shared_ptr<BaseDevice> device);

        /**
         * @return true if the action has been started, but has not finished or been stopped.
         */
        bool isRunning();

        /**
         * Get the rate process function is configured to be called at
         * 
         * @return period between process function calls in milliseconds
         */
        int32_t getProcessPeriodMs();

        /**
         * Set the rate the process function should run at.
         * Must be configured before an action is started to take effect.
         * 
         * @param processPeriodMs New process period in milliseconds
         */
        void setProcessPeriodMs(int32_t processPeriodMs);

    protected:

        /**
         * Run when the action is started.
         */
        virtual void begin() = 0;

        /**
         * Run periodically after the action is started, but before it finishes.
         */
        virtual void process() = 0;

        /**
         * Run when the action is stopped.
         * @param wasInterrupted Will be true if the action did not stop on its own (see Action::shouldContinue)
         */
        virtual void finish(bool wasInterrupted) = 0;

        /**
         * Called each time after Action::process. When this returns false the action will stop.
         * @return false if the action should stop. Else true.
         */
        virtual bool shouldContinue() = 0;

    private:

        void actionStart();

        void actionStop(bool interrupted);

        void actionProcess();

        bool isStarted();
    
        bool isFinished();

        std::shared_ptr<Task> _schedulerTask = nullptr;
        int32_t processRateMs = -1;

        std::vector<BaseDevice*> lockedDevices;

        std::mutex stateLock;
        bool started = false;
        bool finished = false;

        friend class ActionManager;
        friend class ActionSeries;
    };

}