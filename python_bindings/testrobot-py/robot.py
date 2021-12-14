from arpirobot.core.action import Action, ActionManager
from arpirobot.core.robot import BaseRobot
from arpirobot.core.log import Logger
from arpirobot.devices.gamepad import Gamepad, ButtonPressedTrigger
from actions import JSDriveAction, SpeakAction


class Robot(BaseRobot):
    def __init__(self):
        super().__init__()
        self.gp0 = Gamepad(0)
    
    def robot_started(self):
        ActionManager.start_action(JSDriveAction())
        ActionManager.add_trigger(ButtonPressedTrigger(self.gp0, 0, SpeakAction()))


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