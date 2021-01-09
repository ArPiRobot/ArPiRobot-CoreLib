#pragma once

#include <arpirobot/core/device.hpp>
#include <arpirobot/core/scheduler.hpp>
#include <chrono>
#include <thread>
#include <mutex>
#include <vector>

namespace arpirobot{

    struct RobotProfile{
        int mainSchedulerThreads = 10;
        int periodicFunctionRate = 50;
        int maxGamepadDataAge = 100;
    };

    class BaseRobot{
    public:
        BaseRobot(RobotProfile profile);

        void start();

        void feedWatchdog();

        static void beginWhenReady(BaseDevice *device);

        virtual void robotStarted() = 0;

        virtual void robotEnabled() = 0;

        virtual void robotDisabled() = 0;

        virtual void enabledPeriodic() = 0;

        virtual void disabledPeriodic() = 0;

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
        Scheduler scheduler;

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