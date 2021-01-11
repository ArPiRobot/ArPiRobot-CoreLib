#include <actions.hpp>
#include <main.hpp>

#include <arpirobot/core/log.hpp>

void DemoAction::begin(){
    Logger::logDebug("Action Started Here");
}

void DemoAction::process(){
    
}

void DemoAction::finish(bool wasInterrupted){
    
}

bool DemoAction::shouldContinue(){
    return false;
}
