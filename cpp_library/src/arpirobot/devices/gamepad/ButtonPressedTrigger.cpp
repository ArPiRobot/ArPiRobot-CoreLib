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


#include <arpirobot/devices/gamepad/ButtonPressedTrigger.hpp>

using namespace arpirobot;

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