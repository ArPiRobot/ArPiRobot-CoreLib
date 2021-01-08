from arpirobot.core.robot import BaseRobot
from arpirobot.core.network import NetworkTable
from arpirobot.core.log import Logger
import time


class MyRobot(BaseRobot):
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


robot = MyRobot()
robot.start()