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
import arpirobot.DestroyableObject;

import com.sun.jna.Pointer;

public class ActionSeries extends DestroyableObject {

    public ActionSeries(Action[] actions, Action finishedAction){
        verifyNotDestroyed();
        Pointer[] actionsInternal = new Pointer[actions.length];
        for(int i = 0; i < actions.length; ++i){
            actionsInternal[i] = actions[i]._ptr();
        }
        ptr = Bridge.arpirobot.ActionSeries_create(Bridge.ptrArrayToPtr(actionsInternal), actionsInternal.length, 
                finishedAction._ptr());
    }

    @Override
    protected void doDestroy() {
        verifyNotDestroyed();
        Bridge.arpirobot.ActionSeries_destroy(ptr);
    }
    
}
