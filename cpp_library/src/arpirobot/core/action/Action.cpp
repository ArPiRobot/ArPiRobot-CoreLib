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

#include <algorithm>

using namespace arpirobot;

Action::Action(int32_t processRateMs) : processRateMs(processRateMs) {

}

Action::~Action(){
    for(auto dev : currentlyLocked){
        // Ensures BaseDevice never tries to call 
        // lockDevice on ptr to deallocated action
        dev.get().releaseDevice(this);
    }
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

LockedDeviceList Action::lockedDevices(){
    return {};
}

bool Action::predDevice(std::reference_wrapper<BaseDevice> a, std::reference_wrapper<BaseDevice> b){
    return (&(a.get())) == (&(b.get()));
}

bool Action::compDevice(std::reference_wrapper<BaseDevice> a, std::reference_wrapper<BaseDevice> b){
    return (&(a.get())) < (&(b.get()));
}

void Action::makeDeviceListUnique(std::vector<std::reference_wrapper<BaseDevice>> &list){
    std::sort(list.begin(), list.end(), &Action::compDevice);
    list.erase(std::unique(list.begin(), list.end(), &Action::predDevice), list.end());
}

void Action::actionStart(bool skipLock){
    {
        std::lock_guard<std::mutex> l(stateLock);
        started = true;
        finished = false;
    }

    if(!skipLock){
        try{
            currentlyLocked = lockedDevices();
            makeDeviceListUnique(currentlyLocked);
            for(auto &dev : currentlyLocked){
                dev.get().lockDevice(this);
            }
        }catch(const std::runtime_error &e){
            Logger::logWarning("Action encountered exception when running lockedDevices().");
            Logger::logDebug(e.what());
            currentlyLocked = {};
        }
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
    for(auto dev : currentlyLocked){
        dev.get().releaseDevice(this);
    }
    currentlyLocked.clear();
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