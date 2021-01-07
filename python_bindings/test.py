from arpirobot.core.robot import BaseRobot
import time


class MyRobot(BaseRobot):
    def robot_started(self):
        print("robot_started")
    
    def robot_enabled(self):
        print("robot_enabled")
    
    def robot_disabled(self):
        print("robot_disabled")

    def enabled_periodic(self):
        print("enabled_periodic")
    
    def disabled_periodic(self):
        print("disabled_periodic")
    
    def periodic(self):
        print("periodic")
        self.feed_watchdog()


robot = MyRobot()

while True:
    time.sleep(1)