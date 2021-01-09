#include <arpirobot/devices/gamepad.hpp>
#include <arpirobot/core/network.hpp>
#include <arpirobot/core/robot.hpp>

using namespace arpirobot;

Gamepad::Gamepad(int controllerNum) : controllerNum(controllerNum){
    BaseRobot::beginWhenReady(this);
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

void Gamepad::_enable(){

}

void Gamepad::_disable(){

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
    if(axisNum > data->axisCount){
        // Requested axis does not exist
        return 0;
    }
    int ageMillis = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - data->lastUpdateTime).count();
    if(ageMillis > BaseRobot::currentRobot->profile.maxGamepadDataAge){
        // Data too old
        return 0;
    }

    // Get value and apply deadband
    double value = data->axes[axisNum];
    if(std::abs(value) < deadband){
        value = 0;
    }else{
        // Linearly scale from (deadban, 0) to (1, 1)
        value = (value - (std::abs(value) / value * deadband)) / (1 - deadband);
    }

    // TODO: Apply axis transform

    return value;
}

bool Gamepad::getButton(int buttonNum){
    auto it = NetworkManager::controllerData.find(controllerNum);
    if(it == NetworkManager::controllerData.end()){
        // No data for this controller
        return false;
    }
    auto &data = NetworkManager::controllerData[controllerNum];
    if(buttonNum > data->buttonCount){
        // Requested button does not exist
        return false;
    }
    int ageMillis = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - data->lastUpdateTime).count();
    if(ageMillis > BaseRobot::currentRobot->profile.maxGamepadDataAge){
        // Data too old
        return false;
    }

    return data->buttons[buttonNum];
}

int Gamepad::getDpad(int dpadNum){
    auto it = NetworkManager::controllerData.find(controllerNum);
    if(it == NetworkManager::controllerData.end()){
        // No data for this controller
        return false;
    }
    auto &data = NetworkManager::controllerData[controllerNum];
    if(dpadNum > data->dpadCount){
        // Requested button does not exist
        return false;
    }
    int ageMillis = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - data->lastUpdateTime).count();
    if(ageMillis > BaseRobot::currentRobot->profile.maxGamepadDataAge){
        // Data too old
        return false;
    }

    return data->dpads[dpadNum];
}

std::string Gamepad::getDeviceName(){
    return "Gamepad(" + std::to_string(controllerNum) + ")";
}

void Gamepad::begin(){
    
}