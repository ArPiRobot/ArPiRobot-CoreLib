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

#include <arpirobot/core/network/NetworkTable.hpp>
#include <arpirobot/core/network/NetworkManager.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/robot/BaseRobot.hpp>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <algorithm>


using namespace arpirobot;
using namespace std::placeholders;


std::unordered_map<std::string, std::string> NetworkTable::data;
std::mutex NetworkTable::lock;
bool NetworkTable::inSync = false;

void NetworkTable::set(std::string key, std::string value){
    setFromRobot(key, value);
}

std::string NetworkTable::get(std::string key){
    std::lock_guard<std::mutex> l(lock);
    auto it = data.find(key);
    if(it != data.end()){
        return data[key];
    }
    return "";
}

bool NetworkTable::has(std::string key){
    std::lock_guard<std::mutex> l(lock);
    auto it = data.find(key);
    return it != data.end();
}


bool NetworkTable::isInSync(){
    return inSync;
}

void NetworkTable::startSync(){
    lock.lock();
    inSync = true;
    Logger::logDebug("Starting sync from robot to DS.");

    if(!NetworkManager::sendNtRaw(asio::buffer(NET_TABLE_START_SYNC_DATA, 3))){
        // abortSync will be called by handleDisconnect and lock will be released
        return;
    }

    sendAllValues();
    if(!inSync){
        return;
    }

    Logger::logDebug("Ending sync from robot to DS. Waiting for DS to sync data to robot.");
    NetworkManager::sendNtRaw(asio::buffer(NET_TABLE_END_SYNC_DATA, 4));
}

void NetworkTable::sendAllValues(){
    for(const auto &it : data){
        if(!inSync){
            return;
        }
        NetworkManager::sendNt(it.first, it.second);
    }
}

void NetworkTable::finishSync(std::unordered_map<std::string, std::string> dataFromDs){
    Logger::logDebug("Got all sync data from DS to robot.");

    for(const auto &it : dataFromDs){
        data[it.first] = it.second;
    }

    inSync = false;
    lock.unlock();
    Logger::logInfo("Network table sync complete.");
}

void NetworkTable::abortSync(){
    inSync = false;
    lock.unlock();
    Logger::logWarning("Network table sync aborted.");
}

void NetworkTable::setFromRobot(std::string key, std::string value){
    // Do not allow \n or 255 in the key or value
    std::replace(key.begin(), key.end(), (char)255, '\0');
    std::replace(key.begin(), key.end(), '\n', '\0');
    {
        std::lock_guard<std::mutex> l(lock);
        data[key] = value;
    }
    NetworkManager::sendNt(key, value);
}

void NetworkTable::setFromDs(std::string key, std::string value){
    std::lock_guard<std::mutex> l(lock);
    data[key] = value;
}
