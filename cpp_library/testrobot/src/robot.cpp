#include <robot.hpp>
#include <arpirobot/core/action/Action.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <actions.hpp>

void Robot::robotStarted(){

}

void Robot::robotEnabled(){
    
}

void Robot::robotDisabled(){

}

void Robot::enabledPeriodic(){

}

void Robot::disabledPeriodic(){

}

void Robot::periodic(){
    feedWatchdog();
}
