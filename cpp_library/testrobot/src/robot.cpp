#include <robot.hpp>

#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/action/ActionManager.hpp>
#include <arpirobot/core/network/NetworkTable.hpp>

#include <string>

using namespace arpirobot;


void Robot::robotStarted(){
    arduino.addDevice(&enc);
    arduino.begin();
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
    // Do not remove this line or some devices will be disabled.
    feedWatchdog();

    // Put some sensor data in the network table
    // This can be viewed in the drive station
    NetworkTable::set("Pos", std::to_string(enc.getPosition()));
    NetworkTable::set("Vel", std::to_string(enc.getVelocity()));
}