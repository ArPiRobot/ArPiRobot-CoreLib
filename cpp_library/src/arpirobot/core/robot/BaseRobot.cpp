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

#include <arpirobot/core/robot/BaseRobot.hpp>
#include <arpirobot/core/robot/RobotProfile.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/network/NetworkManager.hpp>
#include <arpirobot/core/action/ActionManager.hpp>
#include <arpirobot/core/conversions.hpp>
#include <arpirobot/core/io/Io.hpp>


#include <stdexcept>
#include <chrono>
#include <csignal>
#include <cstdlib>


using namespace arpirobot;


bool BaseRobot::stop = false;
std::vector<BaseDevice*> BaseRobot::devicesToBegin;
BaseRobot *BaseRobot::currentRobot = nullptr;

BaseRobot::BaseRobot(RobotProfile profile) : profile(profile) {
    
}

BaseRobot::~BaseRobot(){
    
}

void BaseRobot::start(std::string ioProvider){

    // Make sure conversions helper is configured properly before starting robot
    Conversions::checkBigEndian();

    if(currentRobot != nullptr){
        Logger::logError("Attempted to start a second robot. This is not allowed.");
        return;
    }

    scheduler = new Scheduler(profile.mainSchedulerThreads);
    currentRobot = this;

    try{
        Io::init(ioProvider);
    }catch(std::runtime_error &e){
        Logger::logError("Failed to initialize IO library.");
        Logger::logDebug(e.what());
        exit(1);
    }

    atexit(Io::terminate);

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
        device->disable();
    }

    Logger::logInfo("Robot Started.");

    // Ensure this runs before periodic functions start running
    robotStarted();
    robotDisabled();

    // Start periodic callbacks
    scheduler->addRepeatedTask(std::bind(&BaseRobot::doPeriodic, this), 
        std::chrono::milliseconds(0), 
        std::chrono::milliseconds(profile.periodicFunctionRate));
    scheduler->addRepeatedTask(std::bind(&BaseRobot::modeBasedPeriodic, this),
        std::chrono::milliseconds(0),
        std::chrono::milliseconds(profile.periodicFunctionRate));
    scheduler->addRepeatedTask(&ActionManager::checkTriggers,
        std::chrono::milliseconds(0),
        std::chrono::milliseconds(profile.periodicFunctionRate)); 

    // Just so there is no instant disable of devices when robot starts
    feedWatchdog();
    // Run watchdog on main thread (don't do this on scheduler b/c it could have all threads in use)
    runWatchdog();

    Logger::logInfo("Robot stopping.");
    
    // Make sure this is nullptr before stopping scheduler
    currentRobot = nullptr;

    // Make sure scheduler stops before devices are disabled
    delete scheduler;
    scheduler = nullptr;

    // Disable all devices when robot stops
    for(BaseDevice *device : devices){
        device->disable();
    }

    NetworkManager::stopNetworking();

    // No need to call this here. This will be called at exit (atexit handler)
    // Io::terminate();
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
                        device->enable();
                    }
                }
            }
        }
    }catch(...){

    }
    watchdogMutex.unlock();
}

std::shared_ptr<Task> BaseRobot::scheduleRepeatedFunction(const std::function<void()> &&func, sched_clk::duration rate){
    if(currentRobot == nullptr)
        return nullptr;
    return currentRobot->scheduler->addRepeatedTask(std::move(func), std::chrono::milliseconds(0), rate);
}

void BaseRobot::runOnceSoon(const std::function<void()> &&func){
    if(currentRobot == nullptr)
        return;
    currentRobot->scheduler->addTask(std::move(func), std::chrono::milliseconds(0));
}

void BaseRobot::removeTaskFromScheduler(std::shared_ptr<Task> task){
    if(currentRobot == nullptr)
        return;
    currentRobot->scheduler->removeTask(task);
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
    try{
        if(isEnabled){
            enabledPeriodic();
        }else{
            disabledPeriodic();
        }
    }catch(const std::runtime_error &e){
        Logger::logError("Error running mode based periodic function!");
        Logger::logDebug(e.what());
    }
}

void BaseRobot::doPeriodic(){
    try{
        periodic();
    }catch(const std::runtime_error &e){
        Logger::logError("Error running periodic!");
        Logger::logDebug(e.what());
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
                        device->disable();
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
                device->disable();
        }

        Logger::logInfo("Robot disabled.");
        try{
            robotDisabled();
        }catch(const std::runtime_error &e){
            Logger::logError("Error running robotDisabled!");
            Logger::logDebug(e.what());
        }
        isEnabled = false;
    }
}

void BaseRobot::onEnable(){
    if(!isEnabled){
        NetworkTable::set("robotstate", "ENABLED");

        for(BaseDevice *device : devices){
            if(device->shouldMatchRobotState())
                device->enable();
        }

        Logger::logInfo("Robot enabled.");
        try{
            robotEnabled();
        }catch(const std::runtime_error &e){
            Logger::logError("Error running robotEnabled!");
            Logger::logDebug(e.what());
        }
        isEnabled = true;
    }
}
