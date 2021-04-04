#include <arpirobot/core/device/BaseDevice.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/action/ActionManager.hpp>


using namespace arpirobot;


////////////////////////////////////////////////////////////////////////////////
/// BaseDevice
////////////////////////////////////////////////////////////////////////////////

BaseDevice::~BaseDevice(){
    
}

void BaseDevice::lockDevice(Action *action){
    std::lock_guard<std::mutex> l(actionLock);
    
    // If the same actio nis locking the device, don't stop it as this will cause issues with scheduler jobs
    if(lockingAction != nullptr && lockingAction != action){
        ActionManager::stopAction(lockingAction);
    }
    lockingAction = action;
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