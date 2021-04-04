#include <arpirobot/core/action/BaseActionTrigger.hpp>
#include <arpirobot/core/action/ActionManager.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/robot/BaseRobot.hpp>

using namespace arpirobot;


BaseActionTrigger::BaseActionTrigger(Action *targetAction, bool doRestart) : 
        targetAction(targetAction), doRestart(doRestart){
    
}

void BaseActionTrigger::startTargetAction(){
    if(targetAction != nullptr)
        ActionManager::startAction(targetAction, doRestart);
}