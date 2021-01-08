import arpirobot.bridge as bridge
import ctypes


class NetworkTable:
    @staticmethod
    def set(key: str, value: str):
        bridge.arpirobot.NetworkTable_set(ctypes.c_char_p(key.encode()), ctypes.c_char_p(value.encode()))
    
    @staticmethod
    def get(key: str) -> str:
        res = ctypes.c_char_p(bridge.arpirobot.NetworkTable_get(ctypes.c_char_p(key.encode())))
        return res.value.decode()
    
    @staticmethod
    def has(key: str) -> bool:
        return bridge.arpirobot.NetworkTable_has(ctypes.c_char_p(key.encode())) == 1