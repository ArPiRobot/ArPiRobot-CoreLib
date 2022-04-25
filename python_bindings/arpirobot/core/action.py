"""
Copyright 2021 Marcus Behel

This file is part of ArPiRobot-CoreLib.

ArPiRobot-CoreLib is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ArPiRobot-CoreLib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with ArPiRobot-CoreLib.  If not, see <https://www.gnu.org/licenses/>. 
"""

import arpirobot.bridge as bridge
import ctypes
from abc import ABC, abstractmethod
from arpirobot.core.device import BaseDevice
from typing import List


## Generic action class. User actions should inherit this class and implement the four pure virtual methods
class Action(ABC):
    def __init__(self):

        @ctypes.CFUNCTYPE(None)
        def begin():
            self.begin()
        
        @ctypes.CFUNCTYPE(None)
        def process():
            self.process()
        
        @ctypes.CFUNCTYPE(None, ctypes.c_bool)
        def finish(was_interrupted: bool):
            self.finish(was_interrupted)
        
        @ctypes.CFUNCTYPE(ctypes.c_bool)
        def should_continue() -> bool:
            return self.should_continue()
        
        # Have to keep reference to these or will be garbage collected then seg fault
        self.b_internal = begin
        self.p_internal = process
        self.f_internal = finish
        self.sc_internal = should_continue

        self._ptr = bridge.arpirobot.Action_create(self.b_internal, self.p_internal, self.f_internal, self.sc_internal)
    
    def __del__(self):
        bridge.arpirobot.Action_destroy(self._ptr)
    

    ## Use this action to lock a set of devices. 
    #  This is the same as calling lock_evice once for each device individually.
    #  @param devices A vector of devices to lock
    def lock_devices(self, devices: List[BaseDevice]):
        # Build list of internal pointers for each device
        d = []
        for dev in devices:
            d.append(dev._ptr)
        d_type = ctypes.c_void_p * len(d)
        bridge.arpirobot.Action_lockDevices(self._ptr, d_type(*d), len(d))

    ## Use this action to lock a device. When a device is locked by this action, 
    #  any action previously locking it will be stopped.
    #  @param device The device to lock
    def lock_device(self, device: BaseDevice):
        bridge.arpirobot.Action_lockDevice(self._ptr, device._ptr)

    ## @returns true if the action has been started, but has not finished or been stopped.
    def is_running(self) -> bool:
        return bridge.arpirobot.Action_isRunning(self._ptr)

    ## Set the rate the process function should run at.
    #  Must be configured before an action is started to take effect.
    #  @param processPeriodMs New process period in milliseconds
    def set_process_period_ms(self, process_period_ms: int):
        bridge.arpirobot.Action_setProcessPeriodMs(self._ptr, process_period_ms)

    @abstractmethod
    def begin(self):
        pass

    @abstractmethod
    def process(self):
        pass

    @abstractmethod
    def finish(self, was_interrupted: bool):
        pass

    @abstractmethod
    def should_continue(self) -> bool:
        pass


## Generic action trigger. Triggers are registered with the action manager. When some event occurs
#   a trigger will run the designated action.
class BaseActionTrigger:
    def __init__(self, target_action: Action):
        self._ptr = None
        # Used to hold reference so not deallocated
        self._target_action = target_action


## Static helper class to manage actions and triggers
class ActionManager:
    __started_actions = []
    __added_triggers = []

    ## Start an action
    #  @param action    A pointer to the action to start.
    #  @param doRestart If true starting an action that is already running will restart the action.
    #                   If false the action will continue running uninterrupted.
    #  @returns true if the action was started successfully
    @staticmethod
    def start_action(action: Action) -> bool:
        # Keep a reference to the action or it will be deallocated
        if action not in ActionManager.__started_actions:
            ActionManager.__started_actions.append(action)
        
        return bridge.arpirobot.ActionManager_startAction(action._ptr)

    ## Stop an action (interrupts it)
    #  If the action is not running nothing is done.
    #  @param action A pointer to the action to stop
    #  @returns true if the action was stopped. If false, the action was not running.
    @staticmethod
    def stop_action(action: Action) -> bool:
        # No longer need reference
        if action in ActionManager.__started_actions:
            ActionManager.__started_actions.remove(action)

        return bridge.arpirobot.ActionManager_stopAction(action._ptr)
    
    ## Add a trigger to start an action when some event occurs.
    #  @param trigger A pointer to the trigger to add.
    @staticmethod
    def add_trigger(trigger: BaseActionTrigger):
        # Keep a reference to the trigger or it will be deallocated
        if trigger not in ActionManager.__added_triggers:
            ActionManager.__added_triggers.append(trigger)
        bridge.arpirobot.ActionManager_addTrigger(trigger._ptr)

    ## Remove a trigger
    #  @param trigger A pointer to the trigger to remove
    @staticmethod
    def remove_trigger(trigger: BaseActionTrigger):
        # No longer need reference
        if trigger in ActionManager.__added_triggers:
            ActionManager.__added_triggers.remove(trigger)
        bridge.arpirobot.ActionManager_removeTrigger(trigger._ptr)


## A special action that will run a sequential set of actions (one at a time)
class ActionSeries:
    ## @param actions A vector of actions to run sequentially
    #  @param finishedAction An action to transition to once other actions are complete
    def __init__(self, actions: List[Action], finished_action: Action):

        # Keep references so these are not deallocated
        self.__actions = actions
        self.__finished_action = finished_action

        # List of internal action pointer
        a = []
        for action in actions:
            a.append(action._ptr)
        a_type = ctypes.c_void_p * len(a)
        self._ptr = bridge.arpirobot.ActionSeries_create(a_type(*a), len(a), finished_action._ptr)
    
    def __del__(self):
        bridge.arpirobot.ActionSeries_destroy(self._ptr)
    
    ## @returns true if the action has been started, but has not finished or been stopped.
    def is_running(self) -> bool:
        return bridge.arpirobot.Action_isRunning(self._ptr)

