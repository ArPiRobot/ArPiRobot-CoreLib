from arpirobot.core.robot import BaseRobot
from arpirobot.core.drive import ArcadeDriveHelper, CubicAxisTransform, SquareRootAxisTransform
from arpirobot.devices.adafruitmotorhat import AdafruitMotorHatMotor
from arpirobot.core.action import ActionManager, ActionSeries
from arpirobot.devices.gamepad import Gamepad, ButtonPressedTrigger
from arpirobot.core.log import Logger

from arpirobot.arduino.iface import ArduinoUartInterface
from arpirobot.arduino.sensor import VoltageMonitor, NxpAdafruit9Dof

from actions import JSDriveAction, DriveTimeAction


class Robot(BaseRobot):
    def __init__(self):
        super().__init__()

        self.flmotor = AdafruitMotorHatMotor(3)
        self.rlmotor = AdafruitMotorHatMotor(4)
        self.frmotor = AdafruitMotorHatMotor(2)
        self.rrmotor = AdafruitMotorHatMotor(1)

        self.drive_helper = ArcadeDriveHelper([self.flmotor, self.rlmotor], [self.frmotor, self.rrmotor])

        self.gp0 = Gamepad(0)

        self.arduino = ArduinoUartInterface("/dev/ttyUSB0", 57600)
        self.vmon = VoltageMonitor("A0", 4.85, 30000, 7500)
        self.enc = NxpAdafruit9Dof()
    
    def robot_started(self):
        self.flmotor.set_inverted(True)
        self.frmotor.set_inverted(True)

        self.gp0.set_axis_transform(1, CubicAxisTransform(0, 0.5))
        self.gp0.set_axis_transform(2, SquareRootAxisTransform())

        self.arduino.add_device(self.vmon)
        self.arduino.add_device(self.enc)
        self.arduino.begin()

        self.vmon.make_main_vmon()

        ActionManager.start_action(JSDriveAction())

    def robot_enabled(self):
        pass

    def robot_disabled(self):
        pass

    def enabled_periodic(self):
        pass

    def disabled_periodic(self):
        pass

    def periodic(self):
        Logger.log_info("Rotation = " + str(self.enc.get_gyro_z()))
        self.feed_watchdog()