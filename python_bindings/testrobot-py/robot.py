
from arpirobot.core.action import ActionManager
from arpirobot.core.log import Logger
from arpirobot.core.robot import BaseRobot
from arpirobot.core.drive import CubicAxisTransform, SquareRootAxisTransform
from arpirobot.core.network import NetworkTable
#from arpirobot.devices.adafruitmotorhat import AdafruitMotorHatMotor
from arpirobot.devices.drv8833 import DRV8833Module
from arpirobot.core.drive import ArcadeDriveHelper
from arpirobot.devices.gamepad import Gamepad, ButtonPressedTrigger
#from arpirobot.arduino.sensor import VoltageMonitor, Ultrasonic4Pin
#from arpirobot.arduino.iface import ArduinoUartInterface
from arpirobot.core.audio import AudioManager

from actions import JSDriveAction, PlaySoundAction


class Robot(BaseRobot):
    SPEED_AXIS = 1
    ROTATION_AXIS = 2

    def __init__(self):
        super().__init__()

        # for Pi Zero W  Use
        # Ain1 = GPIO23 (Header pin 16), Ain2 = GPIO24 (Header pin 18)
        # Bin1 = GPIO17 (Header pin 11), Bin2 = GPIO27 (Header pin 13)
        # SLP = GPIO25 (Header pin 22)  
        drv8833_hat =  DRV8833Module(23, 24, 17, 27, 25)
        self.flmotor = drv8833_hat.get_motor_a()
        
#        self.rlmotor = AdafruitMotorHatMotor(2)
#        self.rrmotor = AdafruitMotorHatMotor(4)
        self.frmotor = drv8833_hat.get_motor_b()

#        self.robot_drive = ArcadeDriveHelper([self.flmotor, self.rlmotor], [self.frmotor, self.rrmotor])
        self.robot_drive = ArcadeDriveHelper([self.flmotor], [self.frmotor])

#        self.arduino_one = ArduinoUartInterface("/dev/ttyUSB0", 57600)
#        self.vmon = VoltageMonitor("A0", 4.85, 30000, 7500)
#        self.ultra_mon = Ultrasonic4Pin(7, 8)

        self.gamepad = Gamepad(0)

    def robot_started(self):
        # Configure devices (motors, Arduinos, sensors, etc)
        # Setup event callbacks here if in use
        # Start Action's if needed here
        self.flmotor.set_inverted(True)
#        self.rrmotor.set_inverted(True)
        self.gamepad.set_axis_transform(self.SPEED_AXIS, CubicAxisTransform(0, 0.5))
        self.gamepad.set_axis_transform(self.ROTATION_AXIS, SquareRootAxisTransform())

        # self.arduino_one.add_device(self.vmon)
        # self.arduino_one.add_device(self.ultra_mon)
        # self.arduino_one.begin()

        # self.vmon.make_main_vmon()

        exterminate_action = PlaySoundAction("/home/pi/arpirobot/sound_files/exterminate_dalek.mp3")
        mommy_action = PlaySoundAction("/home/pi/dalek_sounds_local/are you my mommy-2_Dalek-3.mp3")
        schedule_action = PlaySoundAction("/home/pi/dalek_sounds_local/Schedule Slip Detected-2.mp3")
        tapeout_action = PlaySoundAction("/home/pi/dalek_sounds_local/tapeout-Dalek-1.mp3")


        ActionManager.add_trigger(ButtonPressedTrigger(self.gamepad, 0, exterminate_action, True))
        ActionManager.add_trigger(ButtonPressedTrigger(self.gamepad, 1, mommy_action, True))
        ActionManager.add_trigger(ButtonPressedTrigger(self.gamepad, 2, schedule_action, True))
        ActionManager.add_trigger(ButtonPressedTrigger(self.gamepad, 3, tapeout_action, True))


        #ActionManager.start_action(exterminate_action)

        ActionManager.start_action(JSDriveAction())

        # for dev in AudioManager.get_playback_devices():
        #     #print(dev.name) 
        #     if dev.name == "snd_rpi_hifiberry_dac, HifiBerry DAC HiFi pcm5102a-hifi-0":
        #         #AudioManager.play_sound("/home/pi/exterminate_dalek_stereo.wav", dev)
        #         #AudioManager.play_sound("/home/pi/exterminate_dalek_mono.wav", dev)
        #         AudioManager.play_sound("/home/pi/arpirobot/sound_files/exterminate_dalek.mp3", dev)
        #         #AudioManager.play_sound("/home/pi/arpirobot/sound_files/exterminate_dalek_stereo.wav", dev)


    def robot_enabled(self):
        # Run once each time the robot becomes enabled
        pass

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
        self.flmotor.set_brake_mode(brake_mode)
        # self.rlmotor.set_brake_mode(brake_mode)
        # self.rrmotor.set_brake_mode(brake_mode)
        self.frmotor.set_brake_mode(brake_mode)