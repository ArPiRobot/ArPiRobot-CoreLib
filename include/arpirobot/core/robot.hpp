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
        ~BaseRobot();

        void feedWatchdog();

    private:
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
        bool stopping = false; // Signal to thread to stop
        std::mutex watchdogMutex;
        std::chrono::steady_clock::time_point lastWatchdogFeed;
        bool watchdogDidDisable = false;
        std::thread *watchdogThread;
    };

}