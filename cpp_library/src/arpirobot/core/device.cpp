#include <arpirobot/core/device.hpp>
#include <arpirobot/core/log.hpp>
#include <arpirobot/core/action.hpp>


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

////////////////////////////////////////////////////////////////////////////////
/// MotorController
////////////////////////////////////////////////////////////////////////////////

MotorController::~MotorController(){
    
}

bool MotorController::isInverted(){
    return speedFactor == -1;
}

void MotorController::setInverted(bool inverted){
    speed *= speedFactor; // Multiply by old factor (back to base speed)
    speedFactor = inverted ? -1 : 1; // Change factor
    speed *= speedFactor; // Multiply by new factor
}

bool MotorController::isBrakeMode(){
    return brakeMode;
}

void MotorController::setBrakeMode(bool brakeMode){
    this->brakeMode = brakeMode;
}

double MotorController::getSpeed(){
    if(!enabled)
        return 0;
    {
        std::lock_guard<std::mutex> l(lock);
        return speed / speedFactor; // divide by speed factor so this matches what user sets
    }
}

void MotorController::setSpeed(double speed){
    if(!enabled)
        return;
    
    // Limit range of speeds
    if(speed > 1)
        speed = 1;
    if(speed < -1)
        speed = -1;

    {
        std::lock_guard<std::mutex> l(lock);
        this->speed = speed * speedFactor;
        run();
    }
}

bool MotorController::isEnabled(){
    return enabled;
}

bool MotorController::shouldMatchRobotState(){
    return true;
}

bool MotorController::shouldDisableWithWatchdog(){
    return true;
}

void MotorController::enable(){
    if(!initialized)
        return;
    {
        std::lock_guard<std::mutex> l(lock);
        enabled = true;
    }
}

void MotorController::disable(){
    if(!initialized)
        return;
    {
        std::lock_guard<std::mutex> l(lock);
        enabled = false;
        speed = 0;
        run();
    }
}
