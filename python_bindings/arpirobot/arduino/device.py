
import arpirobot.bridge as bridge
import ctypes


class ArduinoDevice:
    def __init__(self):
        self._ptr = None

    def get_device_name(self) -> str:
        res = ctypes.c_char_p(bridge.arpirobot.ArduinoDevice_getDeviceName(self._ptr))
        retval = res.value.decode()
        bridge.arpirobot.freeString(res)
        return retval
