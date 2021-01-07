#include <arpirobot/core/robot.hpp>
#include <arpirobot/core/log.hpp>

#include <chrono>

using namespace arpirobot;

BaseRobot::BaseRobot(RobotCallbacks callbacks, RobotProfile profile) : callbacks(callbacks),
        profile(profile),
        scheduler(profile.mainSchedulerThreads){
    
    // TODO: Start networking

    // Start watchdog. Runs on its own thread to ensure there can never be a situation where all scheduler threads
    //    are busy and watchdog does not run.
    watchdogThread = new std::thread(std::bind(&BaseRobot::runWatchdog, this));


    // TODO: Ensure devices start disabled

    Logger::logInfo("Robot Started.");

    // Ensure this runs before periodic functions start running
    callbacks.robotStarted();

    // Start periodic callbacks
    scheduler.every(std::chrono::milliseconds(50), callbacks.periodic);
    scheduler.every(std::chrono::milliseconds(50), std::bind(&BaseRobot::modeBasedPeriodic, this));
}

BaseRobot::~BaseRobot(){
    stopping = true;
    watchdogThread->join();
    delete watchdogThread;
}

void BaseRobot::feedWatchdog(){
    watchdogMutex.lock();
    try{
        lastWatchdogFeed = std::chrono::steady_clock::now();
    }catch(...){

    }
    watchdogMutex.unlock();
}

void BaseRobot::modeBasedPeriodic(){
    if(isEnabled){
        callbacks.enabledPeriodic();
    }else{
        callbacks.disabledPeriodic();
    }
}

void BaseRobot::runWatchdog(){
    while(!stopping){
        watchdogMutex.lock();
        try{
            int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() 
                - lastWatchdogFeed).count();
            if(elapsed >= 500){
                watchdogDidDisable = true;
                Logger::logWarning("Watchdog disabling devices!");
                // TODO: Log device disabled by watchdog instead of above message
                // TODO: Disable devices as needed
            }
        }catch(...){

        }
        watchdogMutex.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}