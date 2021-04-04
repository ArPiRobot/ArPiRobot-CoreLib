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

package arpirobot;

import arpirobot.core.log.Logger;

import com.sun.jna.Pointer;

/**
 * For internal use only.
 * 
 * In java, there is no guaranteed destructor. This could lead to memory leaks since underlying objects
 * are dynamically allocated. As such, a user callable destroy() method is provided. If an object will
 * go out of scope, the user can call this function. If it is not called, finalize may be called by the GC
 * in which case a warning will be printed.
 */
public abstract class DestroyableObject {
    protected Pointer ptr = null;

    /**
     * USER CODE SHOULD NEVER USE THIS!!!
     */
    public Pointer _ptr(){
        return ptr;
    }

    @Override
    @SuppressWarnings( "deprecation" )
    protected void finalize() throws Throwable {
        if(ptr != null){
            destroy();
        }
        Logger.logWarning("destroy was not called before object went out of scope.");
    }

    public void destroy(){
        if(ptr != null){
            doDestroy();
            ptr = null;
        }
    }

    protected abstract void doDestroy();

    protected void verifyNotDestroyed(){
        if(ptr == null)
            throw new RuntimeException("Cannot run functions on an object after destroy.");
    }
}
