import arpirobot.bridge as bridge
import ctypes
from arpirobot.core.device import MotorController


class L298NMotor(MotorController):
    def __init__(self, in1_pin: int, in2_pin: int, pwm_pin: int):
        super().__init__()
        self._ptr = bridge.arpirobot.L298NMotor_create(in1_pin, in2_pin, pwm_pin)
    
    def __del__(self):
        bridge.arpirobot.L298NMotor_destroy(self._ptr)


class L298NModule:
    def __init__(self, in1_pin: int, in2_pin: int, ena_pin: int, in3_pin: int, in4_pin: int, enb_pin: int):
        self.__motorA = L298NMotor(in1_pin, in2_pin, ena_pin)
        self.__motorB = L298NMotor(in3_pin, in4_pin, enb_pin)
    
    def get_motor_a(self) -> L298NMotor:
        return self.__motorA
    
    def get_motor_b(self) -> L298NMotor:
        return self.__motorB
