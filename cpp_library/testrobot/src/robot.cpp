#include <robot.hpp>
#include <arpirobot/core/action/Action.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <actions.hpp>

void Robot::robotStarted(){
    flmotor.setInverted(true);
    frmotor.setInverted(true);
}

void Robot::robotEnabled(){
    
}

void Robot::robotDisabled(){

}

void Robot::enabledPeriodic(){
    double speed = gp0.getAxis(1, 0.1) * -1;
    double rotation = gp0.getAxis(2, 0.1);
    driveHelper.update(speed, rotation);
}

void Robot::disabledPeriodic(){

}

void Robot::periodic(){
    feedWatchdog();
}
