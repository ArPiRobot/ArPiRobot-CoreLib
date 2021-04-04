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

package arpirobot.core.action;

import arpirobot.Bridge;

/**
 * Static helper class to manage actions and triggers
 */
public class ActionManager {

    /**
     * Start an action
     * @param action    A pointer to the action to start.
     * @param doRestart If true starting an action that is already running will restart the action.
     *                  If false the action will continue running uninterrupted.
     * @return true if the action was started successfully
     */
    public static boolean startAction(Action action){
        return Bridge.arpirobot.ActionManager_startAction(action._ptr());
    }

    /**
     * Stop an action (interrupts it)
     * If the action is not running nothing is done.
     * @param action A pointer to the action to stop
     * @return true if the action was stopped. If false, the action was not running.
     */
    public static boolean stopAction(Action action){
        return Bridge.arpirobot.ActionManager_stopAction(action._ptr());
    }

    /**
     * Add a trigger to start an action when some event occurs.
     * @param trigger A pointer to the trigger to add.
     */
    public static void addTrigger(BaseActionTrigger trigger){
        Bridge.arpirobot.ActionManager_addTrigger(trigger._ptr());
    }

    /**
     * Remove a trigger
     * @param trigger A pointer to the trigger to remove
     */
    public static void removeTrigger(BaseActionTrigger trigger){
        Bridge.arpirobot.ActionManager_removeTrigger(trigger._ptr());
    }
}
