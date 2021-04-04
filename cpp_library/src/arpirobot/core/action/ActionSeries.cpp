#include <arpirobot/core/action/ActionSeries.hpp>
#include <arpirobot/core/action/ActionManager.hpp>
#include <arpirobot/core/log/Logger.hpp>

using namespace arpirobot;


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
    }else if(finishedAction != nullptr){
        ActionManager::startAction(finishedAction);
    }
}

bool ActionSeries::shouldContinue(){
    return index < actions.size();
}