#include <core/robot.hpp>

#include <chrono>

BaseRobot::BaseRobot(RobotCallbacks callbacks, RobotProfile profile) : callbacks(callbacks),
                                                                       profile(profile),
                                                                       scheduler(profile.mainSchedulerThreads){
    
    // TODO: Start networking

    // TODO: Start watchdog

    callbacks.robotStarted();

    scheduler.every(std::chrono::milliseconds(50), callbacks.periodic);
    scheduler.every(std::chrono::milliseconds(50), std::bind(&BaseRobot::modeBasedPeriodic, this));
}

void BaseRobot::modeBasedPeriodic(){
    if(isEnabled){
        callbacks.enabledPeriodic();
    }else{
        callbacks.disabledPeriodic();
    }
}