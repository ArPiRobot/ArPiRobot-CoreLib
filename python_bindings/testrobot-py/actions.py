
from arpirobot.core.audio import AudioManager
from arpirobot.core.log import Logger
from arpirobot.core.action import Action

import main 

class PlaySoundAction(Action):
    def __init__(self, filename: str):
        super().__init__()
        self.filename = filename
    
    def begin(self):
        print("BEGIN")
        AudioManager.play_sound(self.filename)
        print("HERE")

    def process(self):
        print("PROCESS")
    
    def finish(self, was_interrupted: bool):
        print("FINISH")
    
    def should_continue(self) -> bool:
        print("SHOULD_CONTINUE")
        return False