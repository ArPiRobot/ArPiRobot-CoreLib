
from arpirobot.core.action import ActionManager
from arpirobot.core.log import Logger
from arpirobot.core.robot import BaseRobot


class Robot(BaseRobot):
    def __init__(self):
        super().__init__()

    def robot_started(self):
        pass


    def robot_enabled(self):
        # Run once each time the robot becomes enabled
        pass

    def robot_disabled(self):
        # Runs once each time the robot becomes disabled
        pass

    def periodic(self):
        # Always make sure to feed the watchdog to make sure devices don't get disabled
        #NetworkTable.set("distance", str(self.ultra_mon.get_distance()))
        self.feed_watchdog()
    
    def enabled_periodic(self):
        # Runs periodically while robot is enabled
        pass

    def disabled_periodic(self):
        # Runs periodically while robot is disabled
        pass

    def set_brakemode(self, brake_mode: bool):
        pass