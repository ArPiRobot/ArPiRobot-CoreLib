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
from abc import ABC, abstractmethod
import ctypes
from arpirobot import util


## Settings to configure general robot behavior
class RobotProfileSingleton:
    @property
    def main_scheduler_threads(self) -> int:
        return bridge.arpirobot.RobotProfile_getMainSchedulerThreads()
    
    @main_scheduler_threads.setter
    def main_scheduler_threads(self, value: int):
        bridge.arpirobot.RobotProfile_setMainSchedulerThreads(value)
    
    @property
    def periodic_function_rate(self) -> int:
        return bridge.arpirobot.RobotProfile_getPeriodicFunctionRate()
    
    @periodic_function_rate.setter
    def periodic_function_rate(self, value: int):
        bridge.arpirobot.RobotProfile_setPeriodicFunctionRate(value)
    
    @property
    def max_gamepad_data_age(self) -> int:
        return bridge.arpirobot.RobotProfile_getMaxGamepadDataAge()
    
    @max_gamepad_data_age.setter
    def max_gamepad_data_age(self, value: int):
        bridge.arpirobot.RobotProfile_setMaxGamepadDataAge(value)
    
    @property
    def action_function_period(self):
        return bridge.arpirobot.RobotProfile_getActionFunctionPeriod()
    
    @action_function_period.setter
    def action_function_period(self, value: int):
        bridge.arpirobot.RobotProfile_setActionFunctionPeriod(value)
    
    @property
    def device_watchdog_dur(self):
        return bridge.arpirobot.RobotProfile_getDeviceWatchdogDur()
    
    @device_watchdog_dur.setter
    def device_watchdog_dur(self, value: int):
        bridge.arpirobot.RobotProfile_setDeviceWatchdogDur(value)
    

    @property
    def io_provider(self) -> str:
        res = ctypes.c_char_p(bridge.arpirobot.RobotProfile_getIoProvider())
        retval = res.value.decode()
        bridge.arpirobot.freeString(res)
        return retval
    
    @io_provider.setter
    def io_provider(self, value: str):
        bridge.arpirobot.RobotProfile_setIoProvider(ctypes.c_char_p(value.encode()))


RobotProfile = RobotProfileSingleton()


## Base class to facilitate general robot code structure/flow
#  Each robot program should have a single class that inherits from this class and
#  implements the pure virtual functions.
class BaseRobot(ABC):
    def __init__(self):

        @ctypes.CFUNCTYPE(None)
        def robot_started():
            util._enable_debugpy_this_thread()
            self.robot_started()
        
        @ctypes.CFUNCTYPE(None)
        def robot_stopped():
            util._enable_debugpy_this_thread()
            # Due to camera objects holding references
            # To arbitrary functions in python, it is necessary to
            # Ensure that these are cleared before python cleans
            # up memory of the objects those functions might be in
            # Consider robot with a member camera object that has a callback
            # to a function in that class when ctrl+c pressed
            # - Robot class destructor in python runs
            # - Running pipeline gets a frame and runs callback, which the interpreter will now not run
            # - The gstreamer thread now hangs
            # - Python destructs member camera
            # - This calls destructor for c++ object
            # - gst pipeline state is set to Null as part of shutdown
            # - but thread is blocked, so this doesn't return
            # You can ctrl+c this a second time, but it doesn't allow a clean
            # shutdown.
            # Since most frame callbacks will either be in scope when robot_stopped
            # is called this avoids the issue in most (maybe all since this will be
            # called before interpreter shuts things down) cases
            from arpirobot.core.camera import BaseCamera
            BaseCamera._null_all_callbacks()
            self.robot_stopped()
        
        @ctypes.CFUNCTYPE(None)
        def robot_enabled():
            util._enable_debugpy_this_thread()
            self.robot_enabled()
        
        @ctypes.CFUNCTYPE(None)
        def robot_disabled():
            util._enable_debugpy_this_thread()
            self.robot_disabled()
        
        @ctypes.CFUNCTYPE(None)
        def enabled_periodic():
            util._enable_debugpy_this_thread()
            self.enabled_periodic()
        
        @ctypes.CFUNCTYPE(None)
        def disabled_periodic():
            util._enable_debugpy_this_thread()
            self.disabled_periodic()
        
        @ctypes.CFUNCTYPE(None)
        def periodic():
            util._enable_debugpy_this_thread()
            self.periodic()
        
        # Have to keep reference to these or will be garbage collected then seg fault
        self.rs_internal = robot_started
        self.rsp_insternal = robot_stopped
        self.re_internal = robot_enabled
        self.rd_internal = robot_disabled
        self.ep_internal = enabled_periodic
        self.dp_internal = disabled_periodic
        self.p_internal = periodic

        self._ptr = bridge.arpirobot.BaseRobot_create(
            self.rs_internal, self.rsp_insternal, self.re_internal, self.rd_internal, 
             self.ep_internal, self.dp_internal, self.p_internal)
    
    def __del__(self):
        bridge.arpirobot.BaseRobot_destroy(self._ptr)

    ## Start the robot. Only one robot instance my run at a time
    def start(self):
        bridge.arpirobot.BaseRobot_start(self._ptr)

    ## Feed the watchdog so devices don't become disabled
    def feed_watchdog(self):
        bridge.arpirobot.BaseRobot_feedWatchdog(self._ptr)

    ## Run once when the robot is started
    @abstractmethod
    def robot_started(self):
        pass

    ## Run once when the robot is stopping
    @abstractmethod
    def robot_stopped(self):
        pass

    ## Run each time the robot is enabled
    @abstractmethod
    def robot_enabled(self):
        pass

    ## Run each time the robot is disabled
    @abstractmethod
    def robot_disabled(self):
        pass

    ## Run periodically while the robot is enabled
    @abstractmethod
    def enabled_periodic(self):
        pass

    ## Run periodically while the robot is disabled
    @abstractmethod
    def disabled_periodic(self):
        pass

    ## Run periodically regardless of robot state
    @abstractmethod
    def periodic(self):
        pass
