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

package arpirobot.devices.gamepad;

import arpirobot.Bridge;
import arpirobot.core.action.Action;
import arpirobot.core.action.BaseActionTrigger;

public class ButtonReleasedTrigger extends BaseActionTrigger {
    /**
     * @param gamepad The gamepad to check for button releases on
     * @param buttonNum The button number
     * @param targetAction The action to run when this occurs
     */
    public ButtonReleasedTrigger(Gamepad gamepad, int buttonNum, Action targetAction){
        this(gamepad, buttonNum, targetAction, true);
    }

    /**
     * @param gamepad The gamepad to check for button releases on
     * @param buttonNum The button number
     * @param targetAction The action to run when this occurs
     * @param doRestart If true this will restart the action if it is running when this occurs
     */
    public ButtonReleasedTrigger(Gamepad gamepad, int buttonNum, Action targetAction, boolean doRestart){
        ptr = Bridge.arpirobot.ButtonReleasedTrigger_create(gamepad._ptr(), buttonNum, 
                targetAction._ptr(), doRestart);
    }

    @Override
    protected void doDestroy() {
        Bridge.arpirobot.ButtonReleasedTrigger_destroy(ptr);
    }
}
