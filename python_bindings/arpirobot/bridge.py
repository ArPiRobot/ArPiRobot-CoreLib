import ctypes

arpirobot = ctypes.CDLL("./libarpirobot-core.so")


# NOTE: If returning dynamically allocated string restype should be ptr (ctypes.c_void_p)
#       Using c_char_p will convert to python bytes then can't call free


################################################################################
# General / Helper
################################################################################

arpirobot.freeString.argtypes = [ctypes.c_char_p]
arpirobot.freeString.restype = None

################################################################################
# BaseRobot Bridge
################################################################################

arpirobot.BaseRobot_create.argtypes = [ctypes.c_void_p, 
    ctypes.c_void_p, 
    ctypes.c_void_p, 
    ctypes.c_void_p, 
    ctypes.c_void_p, 
    ctypes.c_void_p, 
    ctypes.c_int, 
    ctypes.c_int]
arpirobot.BaseRobot_create.restype = ctypes.c_void_p

arpirobot.BaseRobot_destroy.argtypes = [ctypes.c_void_p]
arpirobot.BaseRobot_destroy.restype = None

arpirobot.BaseRobot_start.argtypes = [ctypes.c_void_p]
arpirobot.BaseRobot_start.restype = None

arpirobot.BaseRobot_feedWatchdog.argtypes = [ctypes.c_void_p]
arpirobot.BaseRobot_feedWatchdog.restype = None


################################################################################
# NetworkTable Bridge
################################################################################

arpirobot.NetworkTable_set.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
arpirobot.NetworkTable_set.restype = None

arpirobot.NetworkTable_get.argtypes = [ctypes.c_char_p]
arpirobot.NetworkTable_get.restype = ctypes.c_void_p 

arpirobot.NetworkTable_has.argtypes = [ctypes.c_char_p]
arpirobot.NetworkTable_has.restype = ctypes.c_bool


################################################################################
# Logger Bridge
################################################################################

arpirobot.Logger_logDebug.argtypes = [ctypes.c_char_p]
arpirobot.Logger_logDebug.restype = None

arpirobot.Logger_logInfo.argtypes = [ctypes.c_char_p]
arpirobot.Logger_logInfo.restype = None

arpirobot.Logger_logWarning.argtypes = [ctypes.c_char_p]
arpirobot.Logger_logWarning.restype = None

arpirobot.Logger_logError.argtypes = [ctypes.c_char_p]
arpirobot.Logger_logError.restype = None

arpirobot.Logger_logDebugFrom.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
arpirobot.Logger_logDebugFrom.restype = None

arpirobot.Logger_logInfoFrom.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
arpirobot.Logger_logInfoFrom.restype = None

arpirobot.Logger_logWarningFrom.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
arpirobot.Logger_logWarningFrom.restype = None

arpirobot.Logger_logErrorFrom.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
arpirobot.Logger_logErrorFrom.restype = None

arpirobot.Logger_logNewline.argtypes = []
arpirobot.Logger_logNewline.restype = None


################################################################################
# BaseDevice Bridge
################################################################################

arpirobot.BaseDevice_getDeviceName.argtypes = [ctypes.c_void_p]
arpirobot.BaseDevice_getDeviceName.restype = ctypes.c_void_p


################################################################################
# Gamepad Bridge
################################################################################

arpirobot.Gamepad_create.argtypes = [ctypes.c_int]
arpirobot.Gamepad_create.restype = ctypes.c_void_p

arpirobot.Gamepad_destroy.argtypes = [ctypes.c_void_p]
arpirobot.Gamepad_destroy.restype = None

arpirobot.Gamepad_getControllerNum.argtypes = [ctypes.c_void_p]
arpirobot.Gamepad_getControllerNum.restype = ctypes.c_int

arpirobot.Gamepad_getAxis.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_double]
arpirobot.Gamepad_getAxis.restype = ctypes.c_double

arpirobot.Gamepad_getButton.argtypes = [ctypes.c_void_p, ctypes.c_int]
arpirobot.Gamepad_getButton.restype = ctypes.c_bool

arpirobot.Gamepad_getDpad.argtypes = [ctypes.c_void_p, ctypes.c_int]
arpirobot.Gamepad_getDpad.restype = ctypes.c_int
