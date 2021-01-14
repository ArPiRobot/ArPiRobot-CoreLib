from arpirobot.core.robot import BaseRobot
from arpirobot.core.drive import ArcadeDriveHelper, CubicAxisTransform, SquareRootAxisTransform
from arpirobot.core.action import ActionManager, ActionSeries
from arpirobot.devices.ina260 import INA260PowerSensor
from arpirobot.devices.gamepad import Gamepad, ButtonPressedTrigger
from arpirobot.core.log import Logger

from arpirobot.arduino.iface import ArduinoUartInterface
from arpirobot.arduino.sensor import VoltageMonitor, NxpAdafruit9Dof

from actions import JSDriveAction, DriveTimeAction


class Robot(BaseRobot):
    def __init__(self):
        super().__init__()
        self.gp0 = Gamepad(0)
        self.vmon = INA260PowerSensor()
    
    def robot_started(self):
        self.vmon.make_main_vmon()

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