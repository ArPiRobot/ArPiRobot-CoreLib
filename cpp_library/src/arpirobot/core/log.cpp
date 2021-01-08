#include <arpirobot/core/log.hpp>
#include <arpirobot/core/network.hpp>
#include <iostream>

using namespace arpirobot;

std::mutex Logger::logMutex;

void Logger::logDebug(std::string message){
    log("[DEBUG]: " + message);
}
        
void Logger::logInfo(std::string message){
    log("[INFO]: " + message);
}

void Logger::logWarning(std::string message){
    log("[WARNING]: " + message);
}

void Logger::logError(std::string message){
    log("[ERROR]: " + message);
}

void Logger::logDebugFrom(std::string source, std::string message){
    log("[DEBUG]: " + source + " - " + message);
}

void Logger::logInfoFrom(std::string source, std::string message){
    log("[INFO]: " + source + " - " + message);
}

void Logger::logWarningFrom(std::string source, std::string message){
    log("[WARNING]: " + source + " - " + message);
}

void Logger::logErrorFrom(std::string source, std::string message){
    log("[ERROR]: " + source + " - " + message);
}

void Logger::log(std::string message){
    logMutex.lock();
    try{
        std::cout << message << std::endl;
        NetworkManager::sendLogMessage(message + "\n");
    }catch(...){

    }
    logMutex.unlock();
}