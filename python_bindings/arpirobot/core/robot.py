import arpirobot.bridge as bridge
from abc import ABC, abstractmethod
import ctypes


class RobotProfile:
    def __init__(self):
        self.main_scheduler_threads = 10
        self.periodic_function_rate = 50
        self.max_gamepad_data_age = 100


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
            profile.periodic_function_rate, profile.max_gamepad_data_age)
    
    def __del__(self):
        bridge.arpirobot.BaseRobot_destroy(self._ptr)

    def start(self):
        bridge.arpirobot.BaseRobot_start(self._ptr)

    def feed_watchdog(self):
        bridge.arpirobot.BaseRobot_feedWatchdog(self._ptr)

    @abstractmethod
    def robot_started(self):
        pass

    @abstractmethod
    def robot_enabled(self):
        pass

    @abstractmethod
    def robot_disabled(self):
        pass

    @abstractmethod
    def enabled_periodic(self):
        pass

    @abstractmethod
    def disabled_periodic(self):
        pass

    @abstractmethod
    def periodic(self):
        pass
