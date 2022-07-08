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

#include <arpirobot/core/action/ActionSeries.hpp>
#include <arpirobot/core/action/ActionManager.hpp>
#include <arpirobot/core/log/Logger.hpp>

using namespace arpirobot;

ActionSeries::ActionSeries(std::vector<std::reference_wrapper<Action>> actions, Action &finishedAction) : 
        finishedAction(std::shared_ptr<Action>(std::shared_ptr<Action>{}, &finishedAction)){
    for(auto &a : actions){
        this->actions.push_back(std::shared_ptr<Action>(std::shared_ptr<Action>{}, &a.get()));
    }
}

ActionSeries::ActionSeries(std::vector<std::shared_ptr<Action>> actions, Action &finishedAction) : 
        actions(actions), finishedAction(std::shared_ptr<Action>(std::shared_ptr<Action>{}, &finishedAction)){

}

ActionSeries::ActionSeries(std::vector<std::reference_wrapper<Action>> actions, std::shared_ptr<Action> finishedAction) : 
        finishedAction(finishedAction){
    for(auto &a : actions){
        this->actions.push_back(std::shared_ptr<Action>(std::shared_ptr<Action>{}, &a.get()));
    }
}

ActionSeries::ActionSeries(std::vector<std::shared_ptr<Action>> actions, std::shared_ptr<Action> finishedAction) : 
        actions(actions), finishedAction(finishedAction){

}

LockedDeviceList ActionSeries::lockedDevices(){
    // An ActionSeries locks all actions its child actions would lock when the series starts
    // This is used to ensure proper exclusion / interruption of series
    // Consider two actions that lock drive motors to drive shapes. 
    // Assume some actions are delays that lock no devices.
    // If series1 is running then series2 starts, it should always be the case that series1 is interrupted by series2
    // This should happen even if action instances are shared and restarted between the two or 
    // if series1 is in an action (delay) that locks no devices.
    // This is ensured by having series1 lock all devices that will be used during its lifetime when it starts and only
    // releasing them when it finishes

    // No need to worry about duplicates in this list here. Will be handled by Action::actionStart()
    // after it calls this function
    LockedDeviceList list;
    for(auto &act : actions){
        LockedDeviceList sublist = act->lockedDevices();
        list.insert(list.end(), sublist.begin(), sublist.end());
    }
    return list;
}

void ActionSeries::begin(){
    index = 0;

    // Start first action
    if(actions.size() >= 1){
        ActionManager::startActionInternal(actions[index], true, this);
    }
}

void ActionSeries::process(){
    if(index > actions.size())
        return;
    
    // If current action is done start the next one
    try{
        if(actions[index]->isFinished()){
            index += 1;
            if(index < actions.size()){
                ActionManager::startActionInternal(actions[index], true, this);
            }
        }
    }catch(const std::runtime_error &e){
        Logger::logError("An ActionSeries ran into an error handling actions. Its behavior is now unpredictable. The ActionSeries will now be stopped.");
        Logger::logDebug(e.what());
        ActionManager::stopAction(*this);
    }
}

void ActionSeries::finish(bool interrupted){
    if(interrupted){
        ActionManager::stopAction(actions[index]);
    }else if(finishedAction != nullptr){
        ActionManager::startAction(finishedAction);
    }
}

bool ActionSeries::shouldContinue(){
    return index < actions.size();
}