#include <robot.hpp>
#include <actions.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/audio/AudioManager.hpp>
#include <arpirobot/core/network/NetworkTable.hpp>

void Robot::robotStarted(){
    arduino.addDevice(&vmon);
    arduino.addDevice(&lencoder);
    arduino.addDevice(&rencoder);
    arduino.begin();

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
    NetworkTable::set("L Pos", std::to_string(lencoder.getPosition()));
    NetworkTable::set("L Spd", std::to_string(lencoder.getSpeed()));
    feedWatchdog();
}