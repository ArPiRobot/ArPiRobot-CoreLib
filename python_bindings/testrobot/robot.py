from arpirobot.core.robot import BaseRobot
from arpirobot.core.drive import ArcadeDriveHelper, CubicAxisTransform, SquareRootAxisTransform
from arpirobot.devices.adafruitmotorhat import AdafruitMotorHatMotor
from arpirobot.core.action import ActionManager
from arpirobot.devices.gamepad import Gamepad, ButtonReleasedTrigger

from actions import DemoAction


class Robot(BaseRobot):
    def __init__(self):
        super().__init__()

        self.flmotor = AdafruitMotorHatMotor(3)
        self.rlmotor = AdafruitMotorHatMotor(4)
        self.frmotor = AdafruitMotorHatMotor(2)
        self.rrmotor = AdafruitMotorHatMotor(1)

        self.drive_helper = ArcadeDriveHelper([self.flmotor, self.rlmotor], [self.frmotor, self.rrmotor])

        self.gp0 = Gamepad(0)

        self.trigger = ButtonReleasedTrigger(self.gp0, 0, DemoAction())
    
    def robot_started(self):
        self.flmotor.set_inverted(True)
        self.frmotor.set_inverted(True)

        self.gp0.set_axis_transform(1, CubicAxisTransform(0, 0.5))
        self.gp0.set_axis_transform(2, SquareRootAxisTransform())

        ActionManager.add_trigger(self.trigger)
        ActionManager.remove_trigger(self.trigger)

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