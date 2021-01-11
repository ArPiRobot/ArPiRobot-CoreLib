#include <robot.hpp>
#include <arpirobot/core/action.hpp>
#include <arpirobot/devices/gamepad.hpp>
#include <actions.hpp>

void Robot::robotStarted(){
    flmotor.setInverted(true);
    frmotor.setInverted(true);

    ActionManager::addTrigger(new ButtonPressedTrigger(&gp0, 0, new DemoAction()));
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
