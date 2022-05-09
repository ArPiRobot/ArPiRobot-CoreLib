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

void ActionSeries::begin(){
    index = 0;

    // Start first action
    if(actions.size() >= 1){
        ActionManager::startAction(*actions[index]);
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
                ActionManager::startAction(*actions[index]);
            }
        }
    }catch(const std::runtime_error &e){
        Logger::logError("An ActionSeries ran into an error handling actions. Its behavior is now unpredictable. The ActionSeries will now be stopped.");
        Logger::logDebug(e.what());
        ActionManager::stopAction(*this);
    }
}

void ActionSeries::finish(bool interrupted){
    Logger::logDebugFrom("ActionSeries", "finished()");
    if(interrupted){
        ActionManager::stopAction(actions[index]);
    }else if(finishedAction != nullptr){
        ActionManager::startAction(finishedAction);
    }
}

bool ActionSeries::shouldContinue(){
    return index < actions.size();
}