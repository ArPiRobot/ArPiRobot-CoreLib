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

#include <arpirobot/core/action/Action.hpp>
#include <arpirobot/core/action/ActionManager.hpp>
#include <arpirobot/core/log/Logger.hpp>

using namespace arpirobot;

Action::Action(int32_t processRateMs) : processRateMs(processRateMs) {

}

Action::~Action(){
    for(auto dev : lockedDevices){
        // Ensures BaseDevice never tries to call 
        // lockDevice on ptr to deallocated action
        dev->releaseDevice(this);
    }
}

void Action::lockDevices(std::vector<std::reference_wrapper<BaseDevice>> devices){
    for(auto dev : devices){
        lockDevice(dev.get());
    }
}

void Action::lockDevices(std::vector<std::shared_ptr<BaseDevice>> devices){
    for(auto dev : devices){
        lockDevice(dev);
    }
}

void Action::lockDevice(BaseDevice &device){
    lockDevice(std::shared_ptr<BaseDevice>(std::shared_ptr<BaseDevice>{}, &device));
}

void Action::lockDevice(std::shared_ptr<BaseDevice> device){
    device->lockDevice(this);
    lockedDevices.push_back(device.get());
}

bool Action::isRunning(){
    std::lock_guard<std::mutex> l(stateLock);
    return started && !finished;
}

int32_t Action::getProcessPeriodMs(){
    return processRateMs;
}

void Action::setProcessPeriodMs(int32_t processPeriodMs){
    this->processRateMs = processRateMs;
}

void Action::actionStart(){
    {
        std::lock_guard<std::mutex> l(stateLock);
        started = true;
        finished = false;
    }
    try{
        begin();
    }catch(const std::runtime_error &e){
        Logger::logWarning("Action encountered exception when running begin().");
        Logger::logDebug(e.what());
    }
}

void Action::actionStop(bool interrupted){
    // This function is used by ActionManager to stop an action
    {
        std::lock_guard<std::mutex> l(stateLock);
        finished = true;
    }
    try{
        finish(interrupted);
    }catch(const std::runtime_error &e){
        Logger::logWarning("Action encountered exception when running finish().");
        Logger::logDebug(e.what());
    }
    for(auto dev : lockedDevices){
        dev->releaseDevice(this);
    }
    lockedDevices.clear();
}

void Action::actionProcess(){
    {
        // Ensure process cannot run after action finishes
        std::lock_guard<std::mutex> l(stateLock);
        if(!started || finished)
            return;
    }
    
    bool cont  = false;
    try{
        process();
    }catch(const std::runtime_error &e){
        Logger::logWarning("Action encountered exception when running process().");
        Logger::logDebug(e.what());
    }

    try{
        cont = shouldContinue();
    }catch(const std::runtime_error &e){
        Logger::logWarning("Action encountered exception when running shouldContinue().");
        Logger::logDebug(e.what());
    }

    if(!cont){
        ActionManager::stopActionInternal(
            std::shared_ptr<Action>(std::shared_ptr<Action>{}, this), false);
    }
}

bool Action::isStarted(){
    std::lock_guard<std::mutex> l(stateLock);
    return started;
}

bool Action::isFinished(){
    std::lock_guard<std::mutex> l(stateLock);
    return finished;
}