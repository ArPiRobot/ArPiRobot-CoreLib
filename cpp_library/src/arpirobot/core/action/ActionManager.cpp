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
    if(!action->isStarted() || action->isFinished()){
        action->actionStart();
        action->_schedulerTask = BaseRobot::scheduleRepeatedFunction(
            std::bind(&Action::actionProcess, action),
            std::chrono::milliseconds(BaseRobot::currentRobot->profile.actionFunctionPeriod)
        );
        return true;
    }else if(doRestart){
        // Action is already running, but should be restarted
        ActionManager::stopAction(action);
        // Restart action
        action->actionStart();
        action->_schedulerTask = BaseRobot::scheduleRepeatedFunction(
            std::bind(&Action::actionProcess, action),
            std::chrono::milliseconds(BaseRobot::currentRobot->profile.actionFunctionPeriod)
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