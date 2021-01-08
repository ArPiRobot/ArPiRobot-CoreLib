#include <arpirobot/core/device.hpp>
#include <arpirobot/core/log.hpp>

using namespace arpirobot;


////////////////////////////////////////////////////////////////////////////////
/// BaseDevice
////////////////////////////////////////////////////////////////////////////////
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
        return speed;
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

void MotorController::_enable(){
    if(!initialized)
        return;
    {
        std::lock_guard<std::mutex> l(lock);
        enabled = true;
    }
}

void MotorController::_disable(){
    if(!initialized)
        return;
    {
        std::lock_guard<std::mutex> l(lock);
        enabled = false;
        speed = 0;
        run();
    }
}
