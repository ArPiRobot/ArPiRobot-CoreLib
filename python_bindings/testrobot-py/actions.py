
from arpirobot.core.audio import AudioManager
from arpirobot.core.log import Logger
from arpirobot.core.action import Action

import main 

class JSDriveAction(Action):
    def __init__(self):
        # Used to pass settings to the action upon creation
        super().__init__()
    
    def begin(self):
        # Run when the action starts
        # Remember to lock required devices here
        self.lock_device(main.robot.flmotor)
        self.lock_device(main.robot.frmotor)
        # self.lock_device(main.robot.rrmotor)
        # self.lock_device(main.robot.rlmotor)

    def process(self):
        # distance = main.robot.ultra_mon.get_distance()
        speed = main.robot.gamepad.get_axis(main.robot.SPEED_AXIS, 0.1) * -1

        # Run periodically while the action is running
        
        # if(distance < 25 and speed > 0) :
        #     main.robot.set_brakemode(True)           
        #     speed = 0
        # else :
        #     main.robot.set_brakemode(True)           


        #rotation = main.robot.gamepad.get_axis(main.robot.ROTATION_AXIS, 0.1) * 0.85
        # For more rotation torque on wood robot
        rotation = main.robot.gamepad.get_axis(main.robot.ROTATION_AXIS, 0.1) * 1.0
        #Logger.log_debug("{0}, {1}".format(speed,rotation))
        main.robot.robot_drive.update(speed, rotation)
        #main.robot.flmotor.speed = speed


    def finish(self, was_interrupted: bool):
        # Run when the action stops
        main.robot.robot_drive.update(0, 0)

    def should_continue(self) -> bool:
        # Return False when the action should stop
        return True

class PlaySoundAction(Action):
    def __init__(self, filename: str):
        super().__init__()
        self.filename = filename
        self.dev = None
        for dev in AudioManager.get_playback_devices():
            #print(dev.name) 
            if dev.name == "snd_rpi_hifiberry_dac, HifiBerry DAC HiFi pcm5102a-hifi-0":
                self.dev = dev

    def begin(self):
        print("one")
        AudioManager.play_sound(self.filename, self.dev)
        print("two")

    def process(self):
        pass

    def finish(self, was_interrupted: bool):
        print("finish")

    def should_continue(self) -> bool:
        return False

    
    