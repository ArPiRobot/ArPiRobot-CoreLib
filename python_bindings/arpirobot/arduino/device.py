
import arpirobot.bridge as bridge
import ctypes

## Common base class for any device that is connected to an arduino (any device that communicates with the Pi via 
#  an arduino or other microprocessor)
class ArduinoDevice:
    def __init__(self):
        self._ptr = None

    ## Get the human-readable name for this device
    #  @returns The human-readable name
    def get_device_name(self) -> str:
        res = ctypes.c_char_p(bridge.arpirobot.ArduinoDevice_getDeviceName(self._ptr))
        retval = res.value.decode()
        bridge.arpirobot.freeString(res)
        return retval
