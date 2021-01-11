#include <arpirobot/core/action.hpp>
#include <arpirobot/core/log.hpp>
#include <arpirobot/core/robot.hpp>

#include <algorithm>

using namespace arpirobot;


////////////////////////////////////////////////////////////////////////////////
/// Action
////////////////////////////////////////////////////////////////////////////////

void Action::_actionStart(){
    started = true;
    finished = false;
    try{
        begin();
    }catch(const std::runtime_error &e){
        Logger::logWarning("Action encountered exception when running begin().");
        Logger::logDebug(e.what());
    }
}

void Action::_actionStop(bool interrupted){
    // This function is used by ActionManager to stop an action
    finished = true;
    try{
        finish(interrupted);
    }catch(const std::runtime_error &e){
        Logger::logWarning("Action encountered exception when running finish().");
        Logger::logDebug(e.what());
    }
}

void Action::_actionProcess(){
    if(!started || finished)
        return;
    
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
        ActionManager::_stopActionInternal(this, false);
    }
}

void Action::lockDevices(std::vector<BaseDevice*> devices){
    for(auto dev : devices){
        lockDevice(dev);
    }
}

void Action::lockDevice(BaseDevice *device){
    device->_lockDevice(this);
}

bool Action::isRunning(){
    return started && !finished;
}

bool Action::isStarted(){
    return started;
}

bool Action::isFinished(){
    return finished;
}


////////////////////////////////////////////////////////////////////////////////
/// BaseActionTrigger
////////////////////////////////////////////////////////////////////////////////

BaseActionTrigger::BaseActionTrigger(Action *targetAction, bool doRestart){
    targetAction = targetAction;
    this->doRestart = doRestart;
}

void BaseActionTrigger::startTargetAction(){
    if(targetAction != nullptr)
        ActionManager::startAction(targetAction, doRestart);
}

////////////////////////////////////////////////////////////////////////////////
/// ActionManager
////////////////////////////////////////////////////////////////////////////////

std::mutex ActionManager::triggerLock;
std::vector<BaseActionTrigger*> ActionManager::triggers;


bool ActionManager::startAction(Action *action, bool doRestart){
    if(BaseRobot::currentRobot == nullptr)
        return false;
    if(!action->isStarted() || action->isFinished()){
        action->_actionStart();
        action->_schedulerTask = BaseRobot::scheduleRepeatedFunction(
            std::bind(&Action::_actionProcess, action),
            std::chrono::milliseconds(BaseRobot::currentRobot->profile.actionFunctionPeriod)
        );
        return true;
    }else if(doRestart){
        // Action is already running, but should be restarted
        ActionManager::stopAction(action);
        // Restart action
        action->_actionStart();
        action->_schedulerTask = BaseRobot::scheduleRepeatedFunction(
            std::bind(&Action::_actionProcess, action),
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
    return _stopActionInternal(action, true);
}

bool ActionManager::_stopActionInternal(Action *action, bool interrupted){
    if(action->isStarted() && !action->isFinished()){
        action->_actionStop(interrupted);
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

////////////////////////////////////////////////////////////////////////////////
/// ActionSeries
////////////////////////////////////////////////////////////////////////////////

ActionSeries::ActionSeries(std::vector<Action*> actions, Action *finishedAction) : 
        actions(actions), finishedAction(finishedAction){

}

void ActionSeries::begin(){
    index = 0;

    // Start first action
    if(actions.size() >= 1){
        ActionManager::startAction(actions[index]);
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
                ActionManager::startAction(actions[index]);
            }
        }
    }catch(const std::runtime_error &e){
        Logger::logError("An ActionSeries ran into an error handling actions. Its behavior is now unpredictable. The ActionSeries will now be stopped.");
        Logger::logDebug(e.what());
        ActionManager::stopAction(this);
    }
}

void ActionSeries::finish(bool interrupted){
    if(interrupted){
        ActionManager::stopAction(actions[index]);
    }
}

bool ActionSeries::shouldContinue(){
    return index < actions.size();
}
