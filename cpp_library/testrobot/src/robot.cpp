#include <robot.hpp>
#include <actions.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/audio/AudioManager.hpp>

void Robot::robotStarted(){
    
}

void Robot::robotEnabled(){
    int jobc = AudioManager::playSound("C:\\c4.mp3");
    // AudioManager::stopJob(jobc);
    int jobg = AudioManager::playSound("C:\\g4.mp3");
    Logger::logInfo("Job C = " + std::to_string(jobc));
    Logger::logInfo("Job G = " + std::to_string(jobg));
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