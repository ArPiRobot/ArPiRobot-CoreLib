#pragma once

#include <arpirobot/core/device.hpp>
#include <arpirobot/core/scheduler.hpp>
#include <chrono>
#include <thread>
#include <mutex>
#include <vector>

namespace arpirobot{

    /**
     * \class RobotProfile network.hpp arpirobot/core/network.hpp
     * 
     * Settings to configure general robot behavior
     */
    struct RobotProfile{
        /// Number of threads to use on the main scheduler
        int mainSchedulerThreads = 10; 

        /// Rate to run robot periodic functions at (ms)
        int periodicFunctionRate = 50; 

        /// Maximum age gamepad data can be before it is considered invalid (ms)
        int maxGamepadDataAge = 100;  
        
        /// Rate to run action periodic functions at (ms) 
        int actionFunctionPeriod = 50; 
    };

    /**
     * \class BaseRobot network.hpp arpirobot/core/network.hpp
     * 
     * Base class to facilitate general robot code structure/flow
     * Each robot program should have a single class that inherits from this class and
     * implements the pure virtual functions.
     */
    class BaseRobot{
    public:
        BaseRobot(RobotProfile profile = RobotProfile());

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
         * @returns The task on the scheduler. Can be used to stop the task later
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