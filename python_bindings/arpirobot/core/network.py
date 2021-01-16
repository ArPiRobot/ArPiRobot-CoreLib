import arpirobot.bridge as bridge
import ctypes

class MainVmon:
    # Just a common base class for valid main vmon devices
    def __init__(self):
        self._ptr = None
    
    def make_main_vmon(self):
        bridge.arpirobot.MainVMon_makeMainVmon(self._ptr)


class NetworkTable:
    @staticmethod
    def set(key: str, value: str):
        bridge.arpirobot.NetworkTable_set(key.encode(), value.encode())
    
    @staticmethod
    def get(key: str) -> str:
        res = ctypes.c_char_p(bridge.arpirobot.NetworkTable_get(key.encode()))
        retval = res.value.decode()
        bridge.arpirobot.freeString(res)
        return retval
    
    @staticmethod
    def has(key: str) -> bool:
        return bridge.arpirobot.NetworkTable_has(key.encode())