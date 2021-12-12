#include <robot.hpp>
#include <arpirobot/core/action/Action.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/drive/CubicAxisTransform.hpp>
#include <arpirobot/core/drive/SquareRootAxisTransform.hpp>
#include <actions.hpp>
#include <iostream>

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