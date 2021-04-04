#include <arpirobot/core/device/MotorController.hpp>


using namespace arpirobot;


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
