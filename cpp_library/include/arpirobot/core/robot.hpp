#pragma once

#include <arpirobot/core/scheduler.hpp>
#include <chrono>
#include <thread>
#include <mutex>

namespace arpirobot{

    struct RobotProfile{
        int mainSchedulerThreads = 10;
        int periodicFunctionRate = 50;
    };

    class BaseRobot{
    public:
        BaseRobot(RobotProfile profile);

        void start();

        void feedWatchdog();

        virtual void robotStarted() = 0;

        virtual void robotEnabled() = 0;

        virtual void robotDisabled() = 0;

        virtual void enabledPeriodic() = 0;

        virtual void disabledPeriodic() = 0;

        virtual void periodic() = 0;

    private:
        static void sigintHandler(int signal);

        void modeBasedPeriodic();

        void runWatchdog();

        ////////////////////////////////////////////////////////////////////////////
        /// Member variables
        ////////////////////////////////////////////////////////////////////////////

        // Configuration
        RobotProfile profile;

        // Scheduler
        Scheduler scheduler;

        // Status
        bool isEnabled = false;

        // Watchdog
        std::mutex watchdogMutex;
        std::chrono::steady_clock::time_point lastWatchdogFeed;
        bool watchdogDidDisable = false;

        // Stop flag from Ctrl+C
        static bool stop;
    };

}