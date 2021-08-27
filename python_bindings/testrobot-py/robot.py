from arpirobot.core.robot import BaseRobot
from arpirobot.core.drive import ArcadeDriveHelper, CubicAxisTransform, SquareRootAxisTransform
from arpirobot.core.action import ActionManager, ActionSeries
from arpirobot.devices.ina260 import INA260PowerSensor
from arpirobot.devices.gamepad import Gamepad, ButtonPressedTrigger
from arpirobot.core.log import Logger

from arpirobot.arduino.iface import ArduinoUartInterface
from arpirobot.arduino.sensor import VoltageMonitor, NxpAdafruit9Dof
from arpirobot.core.audio import AudioManager


class Robot(BaseRobot):
    def __init__(self):
        super().__init__()
    
    def robot_started(self):
        devs = AudioManager.get_playback_devices()
        for dev in devs:
            print(f"ID: {dev.id}")
            print(f"NAME: {dev.name}")
            print(f"IS_DEFAULT: {dev.is_default}")
            print()

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