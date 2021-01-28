#include <arpirobot/devices/gamepad.hpp>
#include <arpirobot/core/network.hpp>
#include <arpirobot/core/robot.hpp>
#include <arpirobot/core/log.hpp>

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
        if(ageMillis > BaseRobot::currentRobot->profile.maxGamepadDataAge){
            // Data too old
            return 0;
        }

        // Get value and apply deadband
        double value = data->axes[axisNum];
        if(std::abs(value) < deadband){
            value = 0;
        }else if(deadband != 0){
            // Linearly scale from (deadban, 0) to (1, 1)
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
        if(ageMillis > BaseRobot::currentRobot->profile.maxGamepadDataAge){
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
        if(ageMillis > BaseRobot::currentRobot->profile.maxGamepadDataAge){
            // Data too old
            return false;
        }

        return data->dpads[dpadNum];
    }
}

void Gamepad::setAxisTransform(int axisNum, BaseAxisTransform *transform){
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

ButtonPressedTrigger::ButtonPressedTrigger(Gamepad *gamepad, int buttonNum, Action *targetAction, 
        bool doRestart) : BaseActionTrigger(targetAction, doRestart), gamepad(gamepad), buttonNum(buttonNum){

}

bool ButtonPressedTrigger::shouldRun(){
    bool res = false;
    bool value = gamepad->getButton(buttonNum);
    if(value && !lastValue){
        res = true;
    }
    lastValue = value;
    return res;
}


ButtonReleasedTrigger::ButtonReleasedTrigger(Gamepad *gamepad, int buttonNum, Action *targetAction, 
        bool doRestart) : BaseActionTrigger(targetAction, doRestart), gamepad(gamepad), buttonNum(buttonNum){

}

bool ButtonReleasedTrigger::shouldRun(){
    bool res = false;
    bool value = gamepad->getButton(buttonNum);
    if(!value && lastValue){
        res = true;
    }
    lastValue = value;
    return res;
}