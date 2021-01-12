
import arpirobot.bridge as bridge
import ctypes
from typing import Union
from arpirobot.arduino.device import ArduinoDevice


class VoltageMonitor(ArduinoDevice):
    def __init__(self, pin: Union[int, str], vboard: float, r1: int, r2: int, 
            create_device: bool = True, device_id: int = -1):
        super().__init__()
        self._ptr = bridge.arpirobot.VoltageMonitor_create(str(pin).encode(), vboard, r1, r2, create_device, device_id)
        print(self._ptr)
    
    def __del__(self):
        bridge.arpirobot.VoltageMonitor_destroy(self._ptr)
    
    def get_voltage(self) -> float:
        return bridge.arpirobot.VoltageMonitor_getVoltage(self._ptr)
    
    def make_main_vmon(self):
        bridge.arpirobot.VoltageMonitor_makeMainVmon(self._ptr)
