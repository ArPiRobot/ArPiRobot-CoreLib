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

package arpirobot.core.robot;

import arpirobot.Bridge;
import arpirobot.DestroyableObject;

/** 
 * Base class to facilitate general robot code structure/flow
 * Each robot program should have a single class that inherits from this class and
 * implements the pure virtual functions.
 */
public abstract class BaseRobot extends DestroyableObject {

    // Need to keep these in scope, so cannot pass the lambda's directly
    final private Bridge.void_void_func_ptr _robotStarted = () -> robotStarted();
    final private Bridge.void_void_func_ptr _robotEnabled = () -> robotEnabled();
    final private Bridge.void_void_func_ptr _robotDisabled = () -> robotDisabled();
    final private Bridge.void_void_func_ptr _enabledPeriodic = () -> enabledPeriodic();
    final private Bridge.void_void_func_ptr _disabledPeriodic = () -> disabledPeriodic();
    final private Bridge.void_void_func_ptr _periodic = () -> periodic();

    public BaseRobot(){
        this(new RobotProfile());
    }

    public BaseRobot(RobotProfile profile){
        ptr = Bridge.arpirobot.BaseRobot_create(_robotStarted,
                _robotEnabled,
                _robotDisabled,
                _enabledPeriodic,
                _disabledPeriodic,
                _periodic,
                profile.mainSchedulerThreads,
                profile.periodicFunctionRate,
                profile.maxGamepadDataAge,
                profile.actionFunctionPeriod);
    }

    @Override
    protected void doDestroy() {
        Bridge.arpirobot.BaseRobot_destroy(ptr);
    }

    /**
     * Start the robot. Only one robot instance my run at a time
     */
    public void start(){
        verifyNotDestroyed();
        Bridge.arpirobot.BaseRobot_start(ptr);
    }

    /**
     * Feed the watchdog so devices don't become disabled
     */
    public void feedWatchdog(){
        verifyNotDestroyed();
        Bridge.arpirobot.BaseRobot_feedWatchdog(ptr);
    }

    /**
     * Run once when the robot is started
     */
    protected abstract void robotStarted();

    /**
     * Run each time the robot is enabled
     */
    protected abstract void robotEnabled();
    
    /**
     * Run each time the robot is disabled
     */
    protected abstract void robotDisabled();
    
    /**
     * Run periodically while the robot is enabled
     */
    protected abstract void enabledPeriodic();
    
    /**
     * Run periodically while the robot is disabled
     */
    protected abstract void disabledPeriodic();

    /**
     * Run periodically regardless of robot state
     */
    protected abstract void periodic();
}
