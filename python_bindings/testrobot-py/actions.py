from arpirobot.core.action import Action
import time
import main
from arpirobot.core.log import Logger
from arpirobot.core.audio import AudioManager


class JSDriveAction(Action):
    def begin(self):
        Logger.log_info_from("JSDriveAction", "begin")
    
    def process(self):
        pass

    def finish(self, was_interrupted: bool):
        Logger.log_info_from("JSDriveAction", "finish")
    
    def should_continue(self) -> bool:
        return True


class SpeakAction(Action):
    def begin(self):
        Logger.log_info_from("SpeakAction", "begin")
        AudioManager.play_sound("/home/marcus/test.wav")
    
    def process(self):
        Logger.log_info_from("SpeakAction", "process")
    
    def finish(self, was_interrupted: bool):
        Logger.log_info_from("SpeakAction", "finish")
    
    def should_continue(self) -> bool:
        return False
