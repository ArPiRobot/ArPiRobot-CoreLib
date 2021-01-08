import arpirobot.bridge as bridge
import ctypes


class Logger:
    @staticmethod
    def log_debug(message: str):
        bridge.arpirobot.Logger_logDebug(ctypes.c_char_p(message.encode()))

    @staticmethod
    def log_info(message: str):
        bridge.arpirobot.Logger_logInfo(ctypes.c_char_p(message.encode()))

    @staticmethod
    def log_warning(message: str):
        bridge.arpirobot.Logger_logWarning(ctypes.c_char_p(message.encode()))

    @staticmethod
    def log_error(message: str):
        bridge.arpirobot.Logger_logError(ctypes.c_char_p(message.encode()))

    @staticmethod
    def log_debug_from(source: str, message: str):
        bridge.arpirobot.Logger_logDebugFrom(ctypes.c_char_p(source.encode()), ctypes.c_char_p(message.encode()))

    @staticmethod
    def log_info_from(source: str, message: str):
        bridge.arpirobot.Logger_logInfoFrom(ctypes.c_char_p(source.encode()), ctypes.c_char_p(message.encode()))

    @staticmethod
    def log_warning_from(source: str, message: str):
        bridge.arpirobot.Logger_logWarningFrom(ctypes.c_char_p(source.encode()), ctypes.c_char_p(message.encode()))

    @staticmethod
    def log_error_from(source: str, message: str):
        bridge.arpirobot.Logger_logErrorFrom(ctypes.c_char_p(source.encode()), ctypes.c_char_p(message.encode()))

    @staticmethod
    def log_newline():
        bridge.arpirobot.Logger_logNewline()