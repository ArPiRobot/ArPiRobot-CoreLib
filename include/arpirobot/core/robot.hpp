#pragma once

#include <Scheduler.h>
#include <chrono>
#include <thread>
#include <mutex>

namespace arpirobot{

    struct RobotProfile{
        int mainSchedulerThreads = 10;
        int periodicFunctionRate = 50;
    };

    struct RobotCallbacks{
        void (*robotStarted)(void);
        void (*robotEnabled)(void);
        void (*robotDisabled)(void);
        void (*enabledPeriodic)(void);
        void (*disabledPeriodic)(void);
        void (*periodic)(void);
    };

    class BaseRobot{
    public:
        BaseRobot(RobotCallbacks callbacks, RobotProfile profile);

        void start();

        void feedWatchdog();

    private:
        static void sigintHandler(int signal);

        void modeBasedPeriodic();

        void runWatchdog();

        ////////////////////////////////////////////////////////////////////////////
        /// Member variables
        ////////////////////////////////////////////////////////////////////////////

        // Configuration
        RobotProfile profile;
        RobotCallbacks callbacks;

        // Scheduler
        Bosma::Scheduler scheduler;

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