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

#include <arpirobot/core/action/ActionManager.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/robot/BaseRobot.hpp>

#include <algorithm>

using namespace arpirobot;


std::mutex ActionManager::triggerLock;
std::vector<BaseActionTrigger*> ActionManager::triggers;


bool ActionManager::startAction(Action *action, bool doRestart){
    if(BaseRobot::currentRobot == nullptr)
        return false;

    auto period = (action->processRateMs < 0) ? 
            std::chrono::milliseconds(BaseRobot::currentRobot->profile.actionFunctionPeriod) :
            std::chrono::milliseconds(action->processRateMs);
    
    if(!action->isStarted() || action->isFinished()){
        action->actionStart();
        action->_schedulerTask = BaseRobot::scheduleRepeatedFunction(
            std::bind(&Action::actionProcess, action),
            period
        );
        return true;
    }else if(doRestart){
        // Action is already running, but should be restarted
        ActionManager::stopAction(action);
        // Restart action
        action->actionStart();
        action->_schedulerTask = BaseRobot::scheduleRepeatedFunction(
            std::bind(&Action::actionProcess, action),
            period
        );
        return true;
    }else{
        Logger::logDebugFrom("ActionManager", "Attempted to start already running action.");
        return false;
    }
}

void ActionManager::addTrigger(BaseActionTrigger *trigger){
    std::lock_guard<std::mutex> l(triggerLock);
    if(std::find(triggers.begin(), triggers.end(), trigger) == triggers.end()){
        triggers.push_back(trigger);
    }
}

void ActionManager::removeTrigger(BaseActionTrigger *trigger){
    std::lock_guard<std::mutex> l(triggerLock);
    auto pos = std::find(triggers.begin(), triggers.end(), trigger);
    if(pos != triggers.end()){
        triggers.erase(pos);
    }
}

bool ActionManager::stopAction(Action *action){
    // This is the user-facing stop action function. 
    // Always consider the action interrupted if the user manually stops it
    return stopActionInternal(action, true);
}

bool ActionManager::stopActionInternal(Action *action, bool interrupted){
    if(action->isStarted() && !action->isFinished()){
        action->actionStop(interrupted);
        BaseRobot::removeTaskFromScheduler(action->_schedulerTask);
        action->_schedulerTask = nullptr;
        return true;
    }
    return false;
}

void ActionManager::checkTriggers(){
    std::lock_guard<std::mutex> l(triggerLock);
    for(auto trigger : triggers){
        if(trigger->shouldRun()){
            trigger->startTargetAction();
        }
    }
}