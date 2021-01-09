from arpirobot.core.robot import BaseRobot
from arpirobot.core.network import NetworkTable
from arpirobot.core.log import Logger
from arpirobot.devices.gamepad import Gamepad
from arpirobot.devices.adafruitmotorhat import AdafruitMotorHatMotor
from arpirobot.core.drive import TankDriveHelper
import time


class MyRobot(BaseRobot):
    def __init__(self):
        super().__init__()
        self.gp0 = Gamepad(0)
        
        self.flmotor = AdafruitMotorHatMotor(3)
        self.rlmotor = AdafruitMotorHatMotor(4)
        self.frmotor = AdafruitMotorHatMotor(2)
        self.rrmotor = AdafruitMotorHatMotor(1)

        self.drive_helper = TankDriveHelper([self.flmotor, self.rlmotor], [self.frmotor, self.rrmotor])
    
    def robot_started(self):
        self.flmotor.set_inverted(True)
        self.frmotor.set_inverted(True)

    def robot_enabled(self):
        pass

    def robot_disabled(self):
        pass

    def enabled_periodic(self):
        left = self.gp0.get_axis(1, 0.1) * -1
        right = self.gp0.get_axis(3, 0.1) * -1
        self.drive_helper.update(left, right)
    
    def disabled_periodic(self):
        pass
    
    def periodic(self):
        self.feed_watchdog()


robot = MyRobot()
robot.start()