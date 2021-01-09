#include <arpirobot/core/robot.hpp>
#include <arpirobot/core/log.hpp>
#include <arpirobot/core/network.hpp>

#include <chrono>
#include <csignal>

#include <pigpio.h>

using namespace arpirobot;


bool BaseRobot::stop = false;
std::vector<BaseDevice*> BaseRobot::devicesToBegin;
BaseRobot *BaseRobot::currentRobot = nullptr;

BaseRobot::BaseRobot(RobotProfile profile) : profile(profile),
        scheduler(profile.mainSchedulerThreads){
    
}

void BaseRobot::start(){

    if(currentRobot != nullptr){
        Logger::logError("Attempted to start a second robot. This is not allowed.");
        return;
    }

    currentRobot = this;

    int err = gpioInitialise();
    if(err < 0){
        Logger::logError("Failed to initialize pigpio.");
        Logger::logDebug("Pigpio error code " + std::to_string(err));
        throw std::runtime_error("Initialization of hardware interfaces failed.");
    }

    // On interrupt set stop = true
    // This will cause runWatchdog to return and cleanup can be run
    signal(SIGINT, &BaseRobot::sigintHandler);

    NetworkManager::startNetworking(std::bind(&BaseRobot::onEnable, this), std::bind(&BaseRobot::onDisable, this));

    NetworkTable::set("robotstate", "DISABLED");

    // Begin any devices that were instantiated before the robot was started
    for(BaseDevice *device : devicesToBegin){
        devices.push_back(device);
        device->doBegin();
    }
    devicesToBegin.clear();

    // Make sure all devices start disabled
    for(BaseDevice *device : devices){
        device->_disable();
    }

    Logger::logInfo("Robot Started.");

    // Ensure this runs before periodic functions start running
    robotStarted();
    robotDisabled();

    // Start periodic callbacks
    scheduler.addRepeatedTask(std::bind(&BaseRobot::periodic, this), 
        std::chrono::milliseconds(0), 
        std::chrono::milliseconds(profile.periodicFunctionRate));
    scheduler.addRepeatedTask(std::bind(&BaseRobot::modeBasedPeriodic, this),
        std::chrono::milliseconds(0),
        std::chrono::milliseconds(profile.periodicFunctionRate));

    // Just so there is no instant disable of devices when robot starts
    feedWatchdog();
    // Run watchdog on main thread (don't do this on scheduler b/c it could have all threads in use)
    runWatchdog();

    Logger::logInfo("Robot stopping.");
    
    // Disable all devices when robot stops
    for(BaseDevice *device : devices){
        device->_disable();
    }

    // Do not call this here. Registered as atexit func by pigpio
    // Calling this here may just cause issues when BaseDevice object destructors run
    // gpioTerminate();

    NetworkManager::stopNetworking();
    currentRobot = nullptr;
}

void BaseRobot::feedWatchdog(){
    watchdogMutex.lock();
    try{
        lastWatchdogFeed = std::chrono::steady_clock::now();
        if(watchdogDidDisable){
            for(BaseDevice *device : devices){
                if(!device->isEnabled() && device->shouldDisableWithWatchdog()){
                    // Don't enable device with watchdog if it should be disabled with robot
                    if(!device->shouldMatchRobotState() or isEnabled){
                        device->_enable();
                    }
                }
            }
        }
    }catch(...){

    }
    watchdogMutex.unlock();
}

void BaseRobot::beginWhenReady(BaseDevice *device){
    // Don't run begin on devices until the robot is started
    if(currentRobot == nullptr){
        devicesToBegin.push_back(device);
    }else{
        currentRobot->devices.push_back(device);
        device->doBegin();
    }
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
                for(BaseDevice *device : devices){
                    if(device->shouldDisableWithWatchdog() && device->isEnabled()){
                        Logger::logWarningFrom(device->getDeviceName(), "Device disabled by watchdog.");
                        device->_disable();
                    }
                }
                watchdogDidDisable = true;
            }
        }catch(...){

        }
        watchdogMutex.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}

void BaseRobot::onDisable(){
    if(isEnabled){
        NetworkTable::set("robotstate", "DISABLED");

        for(BaseDevice *device : devices){
            if(device->shouldMatchRobotState())
                device->_disable();
        }

        Logger::logInfo("Robot disabled.");
        robotDisabled();
        isEnabled = false;
    }
}

void BaseRobot::onEnable(){
    if(!isEnabled){
        NetworkTable::set("robotstate", "ENABLED");

        for(BaseDevice *device : devices){
            if(device->shouldMatchRobotState())
                device->_enable();
        }

        Logger::logInfo("Robot enabled.");
        robotEnabled();
        isEnabled = true;
    }
}
