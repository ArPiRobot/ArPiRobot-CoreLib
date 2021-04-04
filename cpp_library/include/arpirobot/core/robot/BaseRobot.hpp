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
#include <arpirobot/core/robot/RobotProfile.hpp>
#include <arpirobot/core/scheduler.hpp>
#include <chrono>
#include <thread>
#include <mutex>
#include <vector>

namespace arpirobot{

    /**
     * \class BaseRobot BaseRobot.hpp arpirobot/core/robot/BaseRobot.hpp
     * 
     * Base class to facilitate general robot code structure/flow
     * Each robot program should have a single class that inherits from this class and
     * implements the pure virtual functions.
     */
    class BaseRobot{
    public:
        BaseRobot(RobotProfile profile = RobotProfile());

        virtual ~BaseRobot();

        /**
         * Start the robot. Only one robot instance my run at a time
         */
        void start();

        /**
         * Feed the watchdog so devices don't become disabled
         */
        void feedWatchdog();

        /**
         * Schedule a function to be run at a given rate.
         * @param func The function to run
         * @param rate The rate to run at
         * @return The task on the scheduler. Can be used to stop the task later
         */
        static std::shared_ptr<Task> scheduleRepeatedFunction(const std::function<void()> &&func, sched_clk::duration rate);

        /**
         * Run a function on the scheduler as soon as possible (no delay).
         * The given function will only run once
         * This method will return immediately (will not wait for the target function to run)
         * @param func The function to run
         */
        static void runOnceSoon(const std::function<void()> &&func);

        /**
         * Remove the given task (repeated task) from the scheduler
         * @param task The task to remove
         */
        static void removeTaskFromScheduler(std::shared_ptr<Task> task);

        /**
         * Initialize a device to run once the robot is started.
         * If this is called before a robot is started this will not run the device's BaseDevice::begin 
         * immediately, but when BaseRobot::start() is called.
         * If the robot is already started, BaseDevice::begin will be called.
         * @param device The device to begin once the robot is instantiated
         */
        static void beginWhenReady(BaseDevice *device);

        /**
         * Run once when the robot is started
         */
        virtual void robotStarted() = 0;

        /**
         * Run each time the robot is enabled
         */
        virtual void robotEnabled() = 0;

        /**
         * Run each time the robot is disabled
         */
        virtual void robotDisabled() = 0;

        /**
         * Run periodically while the robot is enabled
         */
        virtual void enabledPeriodic() = 0;

        /**
         * Run periodically while the robot is disabled
         */
        virtual void disabledPeriodic() = 0;

        /**
         * Run periodically regardless of robot state
         */
        virtual void periodic() = 0;

        static BaseRobot *currentRobot;
        
        RobotProfile profile;

    private:
        static void sigintHandler(int signal);

        void modeBasedPeriodic();

        void doPeriodic();

        void runWatchdog();

        void onDisable();

        void onEnable();

        ////////////////////////////////////////////////////////////////////////////
        /// Member variables
        ////////////////////////////////////////////////////////////////////////////

        // Scheduler
        Scheduler *scheduler;

        // Status
        bool isEnabled = false;

        // Watchdog
        std::mutex watchdogMutex;
        std::chrono::steady_clock::time_point lastWatchdogFeed;
        bool watchdogDidDisable = false;

        std::vector<BaseDevice*> devices;

        // Static variables

        // Stop flag from Ctrl+C
        static bool stop;

        // Device that need to be started
        // Static b/c this can be populated before there is an instance of BaseRobot
        static std::vector<BaseDevice*> devicesToBegin;
    };
}