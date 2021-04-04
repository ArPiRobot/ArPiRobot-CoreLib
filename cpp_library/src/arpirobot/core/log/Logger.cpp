/*
 * Copyright 2021 Marcus Behel
 *
 * This file is part of ArPiRobot-CoreLib.
 * 
 * ArPiRobot-CoreLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ArPiRobot-CoreLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with ArPiRobot-CoreLib.  If not, see <https://www.gnu.org/licenses/>. 
 */

#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/network/NetworkManager.hpp>
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

void Logger::logNewline(){
    log("");
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