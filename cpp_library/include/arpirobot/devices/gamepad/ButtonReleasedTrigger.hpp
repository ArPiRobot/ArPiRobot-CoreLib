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

#pragma once

#include <arpirobot/devices/gamepad/Gamepad.hpp>
#include <arpirobot/core/action/BaseActionTrigger.hpp>


namespace arpirobot{
    /**
     * \class ButtonReleasedTrigger ButtonReleasedTrigger.hpp arpirobot/devices/gamepad/ButtonReleasedTrigger.hpp
     * Action trigger for when a button on a gamepad is released
     */
    class ButtonReleasedTrigger : public BaseActionTrigger {
    public:
        /**
         * @param gamepad The gamepad to check for button presses on
         * @param buttonNum The button number
         * @param targetAction The action to run when this occurs
         * @param doRestart If true this will restart the action if it is running when this occurs
         */
        ButtonReleasedTrigger(Gamepad *gamepad, int buttonNum, Action *targetAction, bool doRestart = true);
        bool shouldRun() override;
    
    private:
        Gamepad *gamepad;
        int buttonNum;
        bool lastValue = false;
    };

}