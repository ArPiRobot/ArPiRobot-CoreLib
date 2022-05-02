
from arpirobot.core.action import ActionManager
from arpirobot.core.log import Logger
from arpirobot.core.network import NetworkTable
from arpirobot.core.robot import BaseRobot
from arpirobot.devices.adafruitmotorhat import AdafruitMotorHatMotor
from arpirobot.core.drive import ArcadeDriveHelper
from arpirobot.arduino.iface import ArduinoUartInterface
from arpirobot.arduino.sensor import Mpu6050Imu
from actions import TestAction

class Robot(BaseRobot):
    def __init__(self):
        super().__init__()

        self.flmotor = AdafruitMotorHatMotor(3)
        self.rlmotor = AdafruitMotorHatMotor(4)
        self.frmotor = AdafruitMotorHatMotor(2)
        self.rrmotor = AdafruitMotorHatMotor(1)

        self.drive_helper = ArcadeDriveHelper([self.flmotor, self.rlmotor], [self.frmotor, self.rrmotor])

        self.arduino = ArduinoUartInterface("/dev/ttyUSB0", 57600)
        self.imu = Mpu6050Imu()

        self.test_action = TestAction()

    def robot_started(self):
        self.arduino.add_device(self.imu)
        self.arduino.begin()
        self.imu.calibrate(10)

        NetworkTable.set("P", "1.0")
        NetworkTable.set("I", "0.0")
        NetworkTable.set("D", "0.0")

        self.flmotor.set_inverted(True)
        self.frmotor.set_inverted(True)

        self.flmotor.set_brake_mode(True)
        self.frmotor.set_brake_mode(True)
        self.rrmotor.set_brake_mode(True)
        self.rlmotor.set_brake_mode(True)

        self.test_action.set_process_period_ms(20)

    def robot_enabled(self):
        ActionManager.start_action(self.test_action)

    def robot_disabled(self):
        ActionManager.stop_action(self.test_action)

    def periodic(self):
        NetworkTable.set("Gyro", str(self.imu.get_gyro_z()))
        # Always make sure to feed the watchdog to make sure devices don't get disabled
        self.feed_watchdog()
    
    def enabled_periodic(self):
        pass

    def disabled_periodic(self):
        pass
