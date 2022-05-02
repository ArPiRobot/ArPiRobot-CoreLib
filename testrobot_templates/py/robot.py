
from arpirobot.core.action import ActionManager
from arpirobot.core.log import Logger
from arpirobot.core.network import NetworkTable
from arpirobot.core.robot import BaseRobot

class Robot(BaseRobot):
    def __init__(self):
        super().__init__()

    def robot_started(self):
        pass

    def robot_enabled(self):
        pass

    def robot_disabled(self):
        pass

    def periodic(self):
        # Always make sure to feed the watchdog to make sure devices don't get disabled
        self.feed_watchdog()
    
    def enabled_periodic(self):
        pass

    def disabled_periodic(self):
        pass
