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


## Settings to configure general robot behavior
class RobotProfile:
    def __init__(self):
        ## Number of threads to use on the main scheduler
        self.main_scheduler_threads = 10

        ## Rate to run robot periodic functions at (ms)
        self.periodic_function_rate = 50

        ## Maximum age gamepad data can be before it is considered invalid (ms)
        self.max_gamepad_data_age = 100

        ## Rate to run action periodic functions at (ms) 
        self.action_function_period = 50


## Base class to facilitate general robot code structure/flow
#  Each robot program should have a single class that inherits from this class and
#  implements the pure virtual functions.
class BaseRobot(ABC):
    def __init__(self, profile: RobotProfile = RobotProfile()):

        @ctypes.CFUNCTYPE(None)
        def robot_started():
            self.robot_started()
        
        @ctypes.CFUNCTYPE(None)
        def robot_enabled():
            self.robot_enabled()
        
        @ctypes.CFUNCTYPE(None)
        def robot_disabled():
            self.robot_disabled()
        
        @ctypes.CFUNCTYPE(None)
        def enabled_periodic():
            self.enabled_periodic()
        
        @ctypes.CFUNCTYPE(None)
        def disabled_periodic():
            self.disabled_periodic()
        
        @ctypes.CFUNCTYPE(None)
        def periodic():
            self.periodic()
        
        # Have to keep reference to these or will be garbage collected then seg fault
        self.rs_internal = robot_started
        self.re_internal = robot_enabled
        self.rd_internal = robot_disabled
        self.ep_internal = enabled_periodic
        self.dp_internal = disabled_periodic
        self.p_internal = periodic

        self._ptr = bridge.arpirobot.BaseRobot_create(
            self.rs_internal, self.re_internal, self.rd_internal, self.ep_internal, 
            self.dp_internal, self.p_internal, profile.main_scheduler_threads, 
            profile.periodic_function_rate, profile.max_gamepad_data_age,
            profile.action_function_period)
    
    def __del__(self):
        bridge.arpirobot.BaseRobot_destroy(self._ptr)

    ## Start the robot. Only one robot instance my run at a time
    def start(self, io_provider = ""):
        bridge.arpirobot.BaseRobot_start(self._ptr, io_provider.encode())

    ## Feed the watchdog so devices don't become disabled
    def feed_watchdog(self):
        bridge.arpirobot.BaseRobot_feedWatchdog(self._ptr)

    ## Run once when the robot is started
    @abstractmethod
    def robot_started(self):
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
