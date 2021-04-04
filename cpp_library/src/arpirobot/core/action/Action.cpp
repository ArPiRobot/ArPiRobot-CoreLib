#include <arpirobot/core/action/Action.hpp>
#include <arpirobot/core/action/ActionManager.hpp>
#include <arpirobot/core/log/Logger.hpp>

using namespace arpirobot;


Action::~Action(){
    
}

void Action::lockDevices(std::vector<BaseDevice*> devices){
    for(auto dev : devices){
        lockDevice(dev);
    }
}

void Action::lockDevice(BaseDevice *device){
    device->lockDevice(this);
}

bool Action::isRunning(){
    return started && !finished;
}

void Action::actionStart(){
    {
        std::lock_guard<std::mutex> l(stateLock);
        started = true;
        finished = false;
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
        ActionManager::stopActionInternal(this, false);
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