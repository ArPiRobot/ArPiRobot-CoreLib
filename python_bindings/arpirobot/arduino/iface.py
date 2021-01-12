import arpirobot.bridge as bridge
import ctypes
from arpirobot.arduino.device import ArduinoDevice


class BaseArduinoInterface:
    def __init__(self):
        self._ptr = None
    
    def begin(self):
        bridge.arpirobot.BaseArduinoInterface_begin(self._ptr)
    
    def add_device(self, device: ArduinoDevice):
        bridge.arpirobot.BaseArduinoInterface_addDevice(self._ptr, device._ptr)
    
    def is_ready(self) -> bool:
        return bridge.arpirobot.BaseArduinoInterface_isReady(self._ptr)


class ArduinoUartInterface(BaseArduinoInterface):
    def __init__(self, tty: str, baud: int):
        super().__init__()
        self._ptr = bridge.arpirobot.ArduinoUartInterface_create(tty.encode(), baud)
    
    def __del__(self):
        bridge.arpirobot.ArduinoUartInterface_destroy(self._ptr)
    
