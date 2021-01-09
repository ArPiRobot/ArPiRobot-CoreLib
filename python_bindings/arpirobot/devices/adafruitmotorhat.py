import arpirobot.bridge as bridge
import ctypes
from arpirobot.core.device import MotorController


class AdafruitMotorHatMotor(MotorController):
    DETECT_ADDR = -1
    ADAFRUIT_ADDR = 0x60
    GEEKWORM_ADDR = 0x6F

    def __init__(self, motor_num: int, hat_address: int = DETECT_ADDR, remap_nums: bool = True):
        super().__init__()
        self._ptr = bridge.arpirobot.AdafruitMotorHatMotor_create(motor_num, hat_address, remap_nums)
    
    def __del__(self):
        bridge.arpirobot.AdafruitMotorHatMotor_destroy(self._ptr)
