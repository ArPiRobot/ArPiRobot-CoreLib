from arpirobot.core.robot import BaseRobot
from arpirobot.core.drive import ArcadeDriveHelper, CubicAxisTransform, SquareRootAxisTransform
from arpirobot.core.action import ActionManager, ActionSeries
from arpirobot.devices.ina260 import INA260PowerSensor
from arpirobot.devices.gamepad import Gamepad, ButtonPressedTrigger
from arpirobot.core.log import Logger

from arpirobot.arduino.iface import ArduinoUartInterface
from arpirobot.arduino.sensor import VoltageMonitor, NxpAdafruit9Dof
from arpirobot.core.audio import AudioManager
from arpirobot.devices.gpio import StatusLED


class Robot(BaseRobot):
    def __init__(self):
        super().__init__()

        self.led = StatusLED(12)
    
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