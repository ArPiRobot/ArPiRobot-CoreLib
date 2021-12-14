from arpirobot.core.robot import BaseRobot
from arpirobot.core.log import Logger


class Robot(BaseRobot):
    def __init__(self):
        super().__init__()
    
    def robot_started(self):
        pass


    def robot_enabled(self):
        pass

    def robot_disabled(self):
        pass

    def enabled_periodic(self):
        pass

    def disabled_periodic(self):
        pass

    def periodic(self):
        self.feed_watchdog()