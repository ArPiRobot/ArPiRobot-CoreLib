from arpirobot.core.robot import BaseRobot
from arpirobot.core.drive import ArcadeDriveHelper, CubicAxisTransform, SquareRootAxisTransform
from arpirobot.devices.drv8833 import DRV8833Module
from arpirobot.core.action import ActionManager, ActionSeries
from arpirobot.devices.gamepad import Gamepad, ButtonPressedTrigger
from arpirobot.core.log import Logger

from arpirobot.arduino.iface import ArduinoUartInterface
from arpirobot.arduino.sensor import VoltageMonitor, NxpAdafruit9Dof

from actions import JSDriveAction, DriveTimeAction


class Robot(BaseRobot):
    def __init__(self):
        super().__init__()
        self.gp0 = Gamepad(0)
        self.drv8833 = DRV8833Module(24, 25, 17, 27, 23)
        self.motor1 = self.drv8833.get_motor_a()
        self.motor2 = self.drv8833.get_motor_b()
    
    def robot_started(self):
        pass

    def robot_enabled(self):
        pass

    def robot_disabled(self):
        pass

    def enabled_periodic(self):
        speed = self.gp0.get_axis(1, 0.1)
        self.motor1.set_speed(speed)
        self.motor2.set_speed(speed)

    def disabled_periodic(self):
        pass

    def periodic(self):
        self.feed_watchdog()