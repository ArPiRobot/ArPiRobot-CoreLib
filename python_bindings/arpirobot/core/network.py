import arpirobot.bridge as bridge
import ctypes


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