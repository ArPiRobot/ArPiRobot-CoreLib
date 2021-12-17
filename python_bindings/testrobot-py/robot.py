
from arpirobot.core.action import ActionManager
from arpirobot.core.log import Logger
from arpirobot.core.robot import BaseRobot
from arpirobot.core.audio import AudioManager

from actions import PlaySoundAction

class Robot(BaseRobot):
    def __init__(self):
        super().__init__()

    def robot_started(self):
        self.playSoundAction = PlaySoundAction("/home/pi/sound_files/g4.mp3")


    def robot_enabled(self):
        # Run once each time the robot becomes enabled
        ActionManager.start_action(self.playSoundAction)

    def robot_disabled(self):
        # Runs once each time the robot becomes disabled
        pass

    def periodic(self):
        # Always make sure to feed the watchdog to make sure devices don't get disabled
        #NetworkTable.set("distance", str(self.ultra_mon.get_distance()))
        self.feed_watchdog()
    
    def enabled_periodic(self):
        # Runs periodically while robot is enabled
        pass

    def disabled_periodic(self):
        # Runs periodically while robot is disabled
        pass

    def set_brakemode(self, brake_mode: bool):
        pass