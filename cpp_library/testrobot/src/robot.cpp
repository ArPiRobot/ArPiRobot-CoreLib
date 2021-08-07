#include <robot.hpp>
#include <arpirobot/core/action/Action.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <actions.hpp>
#include <iostream>

void Robot::robotStarted(){
    for(auto dev : AudioManager::getPlaybackDevices()){
        /*if(dev.name == "snd_rpi_hifiberry_dac, HifiBerry DAC HiFi pcm5102a-hifi-0"){
            AudioManager::playSound("/home/pi/StarWars3.wav", dev);
        }*/
        std::cout << "ID: "  << dev.id << std::endl;
        std::cout << "NAME: "  << dev.name << std::endl;
        std::cout << "IS_DEFAULT: "  << dev.isDefault << std::endl;
        std::cout << std::endl;
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