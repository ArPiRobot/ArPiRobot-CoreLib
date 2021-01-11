#include <robot.hpp>
#include <arpirobot/core/action.hpp>
#include <actions.hpp>

void Robot::robotStarted(){
    flmotor.setInverted(true);
    frmotor.setInverted(true);

    ActionManager::startAction(new JSDriveAction());
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
