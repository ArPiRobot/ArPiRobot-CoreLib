#include <robot.hpp>
#include <arpirobot/core/action.hpp>
#include <arpirobot/core/log.hpp>
#include <arpirobot/devices/gamepad.hpp>
#include <actions.hpp>

void Robot::robotStarted(){
    
}

void Robot::robotEnabled(){
    motor1.setSpeed(0.5);
    motor2.setSpeed(0.5);
}

void Robot::robotDisabled(){

}

void Robot::enabledPeriodic(){
    double speed = gp0.getAxis(1, 0.1);
    motor1.setSpeed(speed);
    motor2.setSpeed(speed);
}

void Robot::disabledPeriodic(){

}

void Robot::periodic(){
    feedWatchdog();
}
