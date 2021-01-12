#include <robot.hpp>
#include <arpirobot/core/action.hpp>
#include <arpirobot/core/log.hpp>
#include <actions.hpp>

void Robot::robotStarted(){
    vmon.makeMainVmon();
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
