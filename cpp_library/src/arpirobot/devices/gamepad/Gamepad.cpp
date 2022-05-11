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

#include <arpirobot/devices/gamepad/Gamepad.hpp>
#include <arpirobot/core/network/NetworkManager.hpp>
#include <arpirobot/core/robot/BaseRobot.hpp>
#include <arpirobot/core/log/Logger.hpp>

using namespace arpirobot;

Gamepad::Gamepad(int controllerNum) : controllerNum(controllerNum){
    BaseRobot::beginWhenReady(this);
}

int Gamepad::getControllerNum(){
    return controllerNum;
}

double Gamepad::getAxis(int axisNum, double deadband){
    auto it = NetworkManager::controllerData.find(controllerNum);
    if(it == NetworkManager::controllerData.end()){
        // No data for this controller
        return 0;
    }
    auto &data = NetworkManager::controllerData[controllerNum];

    {
        std::lock_guard<std::mutex> l(data->lock);

        if(axisNum > data->axisCount){
            // Requested axis does not exist
            return 0;
        }
        int ageMillis = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - data->lastUpdateTime).count();
        if(ageMillis > RobotProfile::maxGamepadDataAge){
            // Data too old
            return 0;
        }

        // Get value and apply deadband
        double value = data->axes[axisNum];
        if(std::abs(value) < deadband){
            // If under deadband, return 0.
            // Don't even give value to transform
            return 0;
        }else if(deadband != 0){
            // Linearly scale from (deadband, 0) to (1, 1)
            value = (value - (std::abs(value) / value * deadband)) / (1 - deadband);
        }

        // Apply axis transform (if one exists)
        if(axisTransforms.find(axisNum) != axisTransforms.end()){
            value = axisTransforms[axisNum]->applyTransform(value);
        }

        return value;
    }
}

bool Gamepad::getButton(int buttonNum){
    auto it = NetworkManager::controllerData.find(controllerNum);
    if(it == NetworkManager::controllerData.end()){
        // No data for this controller
        return false;
    }
    auto &data = NetworkManager::controllerData[controllerNum];

    {
        std::lock_guard<std::mutex> l(data->lock);

        if(buttonNum > data->buttonCount){
            // Requested button does not exist
            return false;
        }
        int ageMillis = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - data->lastUpdateTime).count();
        if(ageMillis > RobotProfile::maxGamepadDataAge){
            // Data too old
            return false;
        }

        return data->buttons[buttonNum];
    }
}

int Gamepad::getDpad(int dpadNum){
    auto it = NetworkManager::controllerData.find(controllerNum);
    if(it == NetworkManager::controllerData.end()){
        // No data for this controller
        return false;
    }
    auto &data = NetworkManager::controllerData[controllerNum];

    {
        std::lock_guard<std::mutex> l(data->lock);
        
        if(dpadNum > data->dpadCount){
            // Requested button does not exist
            return false;
        }
        int ageMillis = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - data->lastUpdateTime).count();
        if(ageMillis > RobotProfile::maxGamepadDataAge){
            // Data too old
            return false;
        }

        return data->dpads[dpadNum];
    }
}

void Gamepad::setAxisTransform(int axisNum, BaseAxisTransform &transform){
    setAxisTransform(axisNum, std::shared_ptr<BaseAxisTransform>(std::shared_ptr<BaseAxisTransform>{}, &transform));
}

void Gamepad::setAxisTransform(int axisNum, std::shared_ptr<BaseAxisTransform> transform){
    axisTransforms[axisNum] = transform;
}

void Gamepad::clearAxisTransform(int axisNum){
    if(axisTransforms.find(axisNum) != axisTransforms.end())
        axisTransforms.erase(axisNum);
}

std::string Gamepad::getDeviceName(){
    return "Gamepad(" + std::to_string(controllerNum) + ")";
}

void Gamepad::begin(){

}

bool Gamepad::isEnabled(){
    return true;
}

bool Gamepad::shouldMatchRobotState(){
    return false;
}

bool Gamepad::shouldDisableWithWatchdog(){
    return false;
}

void Gamepad::enable(){

}

void Gamepad::disable(){

}




