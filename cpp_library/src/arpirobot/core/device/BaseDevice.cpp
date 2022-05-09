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

#include <arpirobot/core/device/BaseDevice.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/action/ActionManager.hpp>


using namespace arpirobot;


////////////////////////////////////////////////////////////////////////////////
/// BaseDevice
////////////////////////////////////////////////////////////////////////////////

BaseDevice::~BaseDevice(){
    BaseRobot::deviceDestroyed(this);
}

void BaseDevice::lockDevice(Action *action){
    
    // Do not use this to release the device
    // A different action may have locked it before attempt to release
    if(action == nullptr)
        return;
    
    // If the same action is locking the device, don't stop it as this will cause issues with scheduler jobs
    if(lockingAction != nullptr && lockingAction != action){
        ActionManager::stopAction(*lockingAction);
    }
    {
        std::lock_guard<std::mutex> l(actionLock);
        lockingAction = action;
    }
}

void BaseDevice::releaseDevice(Action *action){
    std::lock_guard<std::mutex> l(actionLock);
    if(lockingAction == action){
        lockingAction = nullptr;
    }
}

bool BaseDevice::isLockedByAction(){
    std::lock_guard<std::mutex> l(actionLock);
    return lockingAction != nullptr;
}

void BaseDevice::doBegin(){
    if(!initialized){
        begin();
        Logger::logDebugFrom(getDeviceName(), "Device started.");
        initialized = true;
    }
}