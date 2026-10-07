"""
Copyright 2021 Marcus Behel

This file is part of ArPiRobot-CoreLib.

ArPiRobot-CoreLib is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ArPiRobot-CoreLib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with ArPiRobot-CoreLib.  If not, see <https://www.gnu.org/licenses/>. 
"""

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