from arpirobot.core.robot import BaseRobot
from arpirobot.core.network import NetworkTable
from arpirobot.core.log import Logger
from arpirobot.devices.gamepad import Gamepad
from arpirobot.devices.adafruitmotorhat import AdafruitMotorHatMotor
import time


class MyRobot(BaseRobot):
    def __init__(self):
        super().__init__()
        self.gp0 = Gamepad(0)
        self.motor = AdafruitMotorHatMotor(1)
    
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