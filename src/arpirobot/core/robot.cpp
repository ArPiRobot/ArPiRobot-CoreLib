#include <arpirobot/core/robot.hpp>
#include <arpirobot/core/log.hpp>

#include <chrono>
#include <csignal>

using namespace arpirobot;


bool BaseRobot::stop = false;


BaseRobot::BaseRobot(RobotProfile profile) : profile(profile),
        scheduler(profile.mainSchedulerThreads){
    signal(SIGINT, &BaseRobot::sigintHandler);
}

void BaseRobot::start(){
    // TODO: Start networking

    // TODO: Ensure devices start disabled

    Logger::logInfo("Robot Started.");

    // Ensure this runs before periodic functions start running
    robotStarted();

    // Start periodic callbacks
    scheduler.every(std::chrono::milliseconds(50), std::bind(&BaseRobot::periodic, this));
    scheduler.every(std::chrono::milliseconds(50), std::bind(&BaseRobot::modeBasedPeriodic, this));

    // Run watchdog on main thread (don't do this on scheduler b/c it could have all threads in use)
    runWatchdog();

    Logger::logInfo("Robot stopping.");
}

void BaseRobot::feedWatchdog(){
    watchdogMutex.lock();
    try{
        lastWatchdogFeed = std::chrono::steady_clock::now();
    }catch(...){

    }
    watchdogMutex.unlock();
}

void BaseRobot::sigintHandler(int signal){
    stop = true;
}

void BaseRobot::modeBasedPeriodic(){
    if(isEnabled){
        enabledPeriodic();
    }else{
        disabledPeriodic();
    }
}

void BaseRobot::runWatchdog(){
    while(!stop){
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