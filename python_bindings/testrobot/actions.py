from arpirobot.core.action import Action
import time
from main import Main
from arpirobot.core.log import Logger


class DemoAction(Action):
    def begin(self):
        Logger.log_debug("DemoAction Started")

    def process(self):
        pass

    def finish(self, interrupted: bool):
        pass

    def should_continue(self) -> bool:
        return True