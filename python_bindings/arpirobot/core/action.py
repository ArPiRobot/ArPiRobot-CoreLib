import arpirobot.bridge as bridge
import ctypes
from abc import ABC, abstractmethod
from arpirobot.core.device import BaseDevice
from typing import List


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
            return self.should_continue();
        
        # Have to keep reference to these or will be garbage collected then seg fault
        self.b_internal = begin
        self.p_internal = process
        self.f_internal = finish
        self.sc_internal = should_continue

        self._ptr = bridge.arpirobot.Action_create(self.b_internal, self.p_internal, self.f_internal, self.sc_internal)
    
    def __del__(self):
        bridge.arpirobot.Action_destroy(self._ptr)
    

    def lock_devices(self, devices: List[BaseDevice]):
        # Build list of internal pointers for each device
        d = []
        for dev in devices:
            d.append(dev._ptr)
        d_type = ctypes.c_void_p * len(d)
        bridge.arpirobot.Action_lockDevices(self._ptr, d_type(*d), len(d))

    def lock_device(self, device: BaseDevice):
        bridge.arpirobot.Action_lockDevice(self._ptr, device._ptr)

    def is_running(self) -> bool:
        return bridge.arpirobot.Action_isRunning(self._ptr)

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


class ActionManager:
    __started_actions = []

    @staticmethod
    def start_action( action: Action) -> bool:
        # Keep a reference to the action or it will be deallocated
        if action not in ActionManager.__started_actions:
            ActionManager.__started_actions.append(action)
        
        return bridge.arpirobot.ActionManager_startAction(action._ptr)

    @staticmethod
    def stop_action( action: Action) -> bool:
        # No longer need reference
        if action in ActionManager.__started_actions:
            ActionManager.__started_actions.remove(action)

        return bridge.arpirobot.ActionManager_stopAction(action._ptr)


class ActionSeries:
    def __init__(self, actions: List[Action], finished_action: Action):
        # List of internal action pointer
        a = []
        for action in actions:
            a.append(action._ptr)
        a_type = ctypes.c_void_p * len(a)
        self._ptr = bridge.arpirobot.ActionSeries_create(a_type(a), len(a), finished_action._ptr)
    
    def __del__(self):
        bridge.arpirobot.ActionSeries_destroy(self._ptr)
