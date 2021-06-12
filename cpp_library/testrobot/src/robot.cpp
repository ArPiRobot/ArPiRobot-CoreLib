#include <robot.hpp>
#include <arpirobot/core/action/Action.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <actions.hpp>

void Robot::robotStarted(){
    for(auto dev : AudioManager::getPlaybackDevices()){
        if(dev.name == "snd_rpi_hifiberry_dac, HifiBerry DAC HiFi pcm5102a-hifi-0"){
            AudioManager::playSound("/home/pi/StarWars3.wav", dev);
        }
    }
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