import arpirobot.bridge as bridge
import ctypes


## Helper class with static methods for logging messages.
#  Log messages will be printed to stdout and sent to a connected DS.
class Logger:

    ## Log a debug message
    #  @param message The message to log
    @staticmethod
    def log_debug(message: str):
        bridge.arpirobot.Logger_logDebug(ctypes.c_char_p(message.encode()))

    ## Log an info message
    #  @param message The message to log
    @staticmethod
    def log_info(message: str):
        bridge.arpirobot.Logger_logInfo(ctypes.c_char_p(message.encode()))

    ## Log a warning message
    #  @param message The message to log
    @staticmethod
    def log_warning(message: str):
        bridge.arpirobot.Logger_logWarning(ctypes.c_char_p(message.encode()))

    ## Log an error message
    #  @param message The message to log
    @staticmethod
    def log_error(message: str):
        bridge.arpirobot.Logger_logError(ctypes.c_char_p(message.encode()))

    ## Log a debug message from a specific source
    #  @param source The source of a message (often a device name)
    #  @param message The messsage to log
    @staticmethod
    def log_debug_from(source: str, message: str):
        bridge.arpirobot.Logger_logDebugFrom(ctypes.c_char_p(source.encode()), ctypes.c_char_p(message.encode()))

    ## Log an info message from a specific source
    #  @param source The source of a message (often a device name)
    #  @param message The messsage to log
    @staticmethod
    def log_info_from(source: str, message: str):
        bridge.arpirobot.Logger_logInfoFrom(ctypes.c_char_p(source.encode()), ctypes.c_char_p(message.encode()))

    ## Log a warning message from a specific source
    #  @param source The source of a message (often a device name)
    #  @param message The messsage to log
    @staticmethod
    def log_warning_from(source: str, message: str):
        bridge.arpirobot.Logger_logWarningFrom(ctypes.c_char_p(source.encode()), ctypes.c_char_p(message.encode()))

    ## Log an error message from a specific source
    #  @param source The source of a message (often a device name)
    #  @param message The messsage to log
    @staticmethod
    def log_error_from(source: str, message: str):
        bridge.arpirobot.Logger_logErrorFrom(ctypes.c_char_p(source.encode()), ctypes.c_char_p(message.encode()))

    ## Add an empty line to the log
    @staticmethod
    def log_newline():
        bridge.arpirobot.Logger_logNewline()