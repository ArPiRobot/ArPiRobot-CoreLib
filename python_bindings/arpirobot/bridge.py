import ctypes

arpirobot = ctypes.CDLL("libarpirobot-core.so")


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
    ctypes.c_int,
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

arpirobot.Gamepad_setAxisTransform.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_void_p]
arpirobot.Gamepad_setAxisTransform.restype = None

arpirobot.Gamepad_clearAxisTransform.argtypes = [ctypes.c_void_p, ctypes.c_int]
arpirobot.Gamepad_clearAxisTransform.restype = None

################################################################################
# ButtonPressedTrigger Bridge
################################################################################

arpirobot.ButtonPressedTrigger_create.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_void_p, ctypes.c_bool]
arpirobot.ButtonPressedTrigger_create.restype = ctypes.c_void_p

arpirobot.ButtonPressedTrigger_destroy.argtypes = [ctypes.c_void_p]
arpirobot.ButtonPressedTrigger_destroy.restype = None


################################################################################
# ButtonReleasedTrigger Bridge
################################################################################

arpirobot.ButtonReleasedTrigger_create.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_void_p, ctypes.c_bool]
arpirobot.ButtonReleasedTrigger_create.restype = ctypes.c_void_p

arpirobot.ButtonReleasedTrigger_destroy.argtypes = [ctypes.c_void_p]
arpirobot.ButtonReleasedTrigger_destroy.restype = None


################################################################################
# MotorController Bridge
################################################################################

arpirobot.MotorController_isInverted.argtypes = [ctypes.c_void_p]
arpirobot.MotorController_isInverted.restype = ctypes.c_bool

arpirobot.MotorController_setInverted.argtypes = [ctypes.c_void_p, ctypes.c_bool]
arpirobot.MotorController_setInverted.restype = None

arpirobot.MotorController_isBrakeMode.argtypes = [ctypes.c_void_p]
arpirobot.MotorController_isBrakeMode.restype = ctypes.c_bool

arpirobot.MotorController_setBrakeMode.argtypes = [ctypes.c_void_p, ctypes.c_bool]
arpirobot.MotorController_setBrakeMode.restype = None

arpirobot.MotorController_setSpeed.argtypes = [ctypes.c_void_p, ctypes.c_double]
arpirobot.MotorController_setSpeed.restype = None

arpirobot.MotorController_getSpeed.argtypes = [ctypes.c_void_p]
arpirobot.MotorController_getSpeed.restype = ctypes.c_double


################################################################################
# AdafruitMotorHatMotor Bridge
################################################################################

arpirobot.AdafruitMotorHatMotor_create.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_bool]
arpirobot.AdafruitMotorHatMotor_create.restype = ctypes.c_void_p

arpirobot.AdafruitMotorHatMotor_destroy.argtypes = [ctypes.c_void_p]
arpirobot.AdafruitMotorHatMotor_destroy.restype = None


################################################################################
# L298NMotor Bridge
################################################################################

arpirobot.L298NMotor_create.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int]
arpirobot.L298NMotor_create.restype = ctypes.c_void_p

arpirobot.L298NMotor_destroy.argtypes = [ctypes.c_void_p]
arpirobot.L298NMotor_destroy.restype = None


################################################################################
# TB6612Motor Bridge
################################################################################

arpirobot.TB6612Motor_create.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int]
arpirobot.TB6612Motor_create.restype = ctypes.c_void_p

arpirobot.TB6612Motor_destroy.argtypes = [ctypes.c_void_p]
arpirobot.TB6612Motor_destroy.restype = None


################################################################################
# DRV8833Motor Bridge
################################################################################

arpirobot.DRV8833Motor_create.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int]
arpirobot.DRV8833Motor_create.restype = ctypes.c_void_p

arpirobot.DRV8833Motor_destroy.argtypes = [ctypes.c_void_p]
arpirobot.DRV8833Motor_destroy.restype = None


################################################################################
# ArcadeDriveHelper Bridge
################################################################################

arpirobot.ArcadeDriveHelper_create.argtypes = [ctypes.c_void_p, ctypes.c_size_t, ctypes.c_void_p, ctypes.c_size_t]
arpirobot.ArcadeDriveHelper_create.restype = ctypes.c_void_p

arpirobot.ArcadeDriveHelper_destroy.argtypes = [ctypes.c_void_p]
arpirobot.ArcadeDriveHelper_destroy.restype = None

arpirobot.ArcadeDriveHelper_updateSpeed.argtypes = [ctypes.c_void_p, ctypes.c_double]
arpirobot.ArcadeDriveHelper_updateSpeed.restype = None

arpirobot.ArcadeDriveHelper_updateRotation.argtypes = [ctypes.c_void_p, ctypes.c_double]
arpirobot.ArcadeDriveHelper_updateRotation.restype = None

arpirobot.ArcadeDriveHelper_update.argtypes = [ctypes.c_void_p, ctypes.c_double, ctypes.c_double]
arpirobot.ArcadeDriveHelper_update.restype = None


################################################################################
# TankDriveHelper Bridge
################################################################################

arpirobot.TankDriveHelper_create.argtypes = [ctypes.c_void_p, ctypes.c_size_t, ctypes.c_void_p, ctypes.c_size_t]
arpirobot.TankDriveHelper_create.restype = ctypes.c_void_p

arpirobot.TankDriveHelper_destroy.argtypes = [ctypes.c_void_p]
arpirobot.TankDriveHelper_destroy.restype = None

arpirobot.TankDriveHelper_updateLeftSpeed.argtypes = [ctypes.c_void_p, ctypes.c_double]
arpirobot.TankDriveHelper_updateLeftSpeed.restype = None

arpirobot.TankDriveHelper_updateRightSpeed.argtypes = [ctypes.c_void_p, ctypes.c_double]
arpirobot.TankDriveHelper_updateRightSpeed.restype = None

arpirobot.TankDriveHelper_update.argtypes = [ctypes.c_void_p, ctypes.c_double, ctypes.c_double]
arpirobot.TankDriveHelper_update.restype = None


################################################################################
# BaseAxisTransform Bridge
################################################################################

arpirobot.BaseAxisTransform_applyTransform.argtypes = [ctypes.c_void_p, ctypes.c_double]
arpirobot.BaseAxisTransform_applyTransform.restype = ctypes.c_double


################################################################################
# SquareRootAxisTransform Bridge
################################################################################

arpirobot.SquareRootAxisTransform_create.argtypes = []
arpirobot.SquareRootAxisTransform_create.restype = ctypes.c_void_p

arpirobot.SquareRootAxisTransform_destroy.argtypes = [ctypes.c_void_p]
arpirobot.SquareRootAxisTransform_destroy.restype = None


################################################################################
# CubicAxisTransform Bridge
################################################################################

arpirobot.CubicAxisTransform_create.argtypes = [ctypes.c_double, ctypes.c_double]
arpirobot.CubicAxisTransform_create.restype = ctypes.c_void_p

arpirobot.CubicAxisTransform_destroy.argtypes = [ctypes.c_void_p]
arpirobot.CubicAxisTransform_destroy.restype = None


################################################################################
# Action Bridge
################################################################################

arpirobot.Action_create.argtypes = [ctypes.c_void_p, ctypes.c_void_p, ctypes.c_void_p, ctypes.c_void_p]
arpirobot.Action_create.restype = ctypes.c_void_p

arpirobot.Action_destroy.argtypes = [ctypes.c_void_p]
arpirobot.Action_destroy.restype = None

arpirobot.Action_lockDevices.argtypes = [ctypes.c_void_p, ctypes.c_void_p, ctypes.c_size_t]
arpirobot.Action_lockDevices.restype = None

arpirobot.Action_lockDevice.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
arpirobot.Action_lockDevice.restype = None

arpirobot.Action_isRunning.argtypes = [ctypes.c_void_p]
arpirobot.Action_isRunning.restype = ctypes.c_bool


################################################################################
# ActionManager Bridge
################################################################################

arpirobot.ActionManager_startAction.argtypes = [ctypes.c_void_p]
arpirobot.ActionManager_startAction.restype = ctypes.c_bool

arpirobot.ActionManager_stopAction.argtypes = [ctypes.c_void_p]
arpirobot.ActionManager_stopAction.restype = ctypes.c_bool


################################################################################
# ActionSeries Bridge
################################################################################

arpirobot.ActionSeries_create.argtypes = [ctypes.c_void_p, ctypes.c_size_t, ctypes.c_void_p]
arpirobot.ActionSeries_create.restype = ctypes.c_void_p

arpirobot.ActionSeries_destroy.argtypes = [ctypes.c_void_p]
arpirobot.ActionSeries_destroy.restype = None


################################################################################
# BaseArduinoInterface Bridge
################################################################################

arpirobot.BaseArduinoInterface_begin.argtypes = [ctypes.c_void_p]
arpirobot.BaseArduinoInterface_begin.restype = None

arpirobot.BaseArduinoInterface_addDevice.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
arpirobot.BaseArduinoInterface_addDevice.restype = None

arpirobot.BaseArduinoInterface_isReady.argtypes = [ctypes.c_void_p]
arpirobot.BaseArduinoInterface_isReady.restype = ctypes.c_bool


################################################################################
# ArduinoUartInterface Bridge
################################################################################

arpirobot.ArduinoUartInterface_create.argtypes = [ctypes.c_char_p, ctypes.c_int]
arpirobot.ArduinoUartInterface_create.restype = ctypes.c_void_p

arpirobot.ArduinoUartInterface_destroy.argtypes = [ctypes.c_void_p]
arpirobot.ArduinoUartInterface_destroy.restype = None


################################################################################
# ArduinoDevice Bridge
################################################################################

arpirobot.ArduinoDevice_getDeviceName.argtypes = [ctypes.c_void_p]
arpirobot.ArduinoDevice_getDeviceName.restype = ctypes.c_void_p


################################################################################
# VoltageMonitor Bridge
################################################################################

arpirobot.VoltageMonitor_create.argtypes = [ctypes.c_char_p, ctypes.c_double, ctypes.c_int, ctypes.c_int, 
    ctypes.c_bool, ctypes.c_int]
arpirobot.VoltageMonitor_create.restype = ctypes.c_void_p

arpirobot.VoltageMonitor_destroy.argtypes = [ctypes.c_void_p]
arpirobot.VoltageMonitor_destroy.restype = None

arpirobot.VoltageMonitor_getVoltage.argtypes = [ctypes.c_void_p]
arpirobot.VoltageMonitor_getVoltage.restype = ctypes.c_double

arpirobot.VoltageMonitor_makeMainVmon.argtypes = [ctypes.c_void_p]
arpirobot.VoltageMonitor_makeMainVmon.restype = None


################################################################################
# Ultrasonic4Pin Bridge
################################################################################

arpirobot.Ultrasonic4Pin_create.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_bool, ctypes.c_int]
arpirobot.Ultrasonic4Pin_create.restype = ctypes.c_void_p

arpirobot.Ultrasonic4Pin_destroy.argtypes = [ctypes.c_void_p]
arpirobot.Ultrasonic4Pin_destroy.restype = None

arpirobot.Ultrasonic4Pin_getDistance.argtypes = [ctypes.c_void_p]
arpirobot.Ultrasonic4Pin_getDistance.restype = ctypes.c_int


################################################################################
# SingleEncoder Bridge
################################################################################

arpirobot.SingleEncoder_create.argtypes = [ctypes.c_char_p, ctypes.c_bool, ctypes.c_bool, ctypes.c_int]
arpirobot.SingleEncoder_create.restype = ctypes.c_void_p

arpirobot.SingleEncoder_destroy.argtypes = [ctypes.c_void_p]
arpirobot.SingleEncoder_destroy.restype = None

arpirobot.SingleEncoder_getPosition.argtypes = [ctypes.c_void_p]
arpirobot.SingleEncoder_getPosition.restype = ctypes.c_int

arpirobot.SingleEncoder_setPosition.argtypes = [ctypes.c_void_p, ctypes.c_int]
arpirobot.SingleEncoder_setPosition.restype = None


################################################################################
# IRReflectorModule Bridge
################################################################################

arpirobot.IRReflectorModule_create.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_bool, ctypes.c_int]
arpirobot.IRReflectorModule_create.restype = ctypes.c_void_p

arpirobot.IRReflectorModule_destroy.argtypes = [ctypes.c_void_p]
arpirobot.IRReflectorModule_destroy.restype = None

arpirobot.IRReflectorModule_getDigitalValue.argtypes = [ctypes.c_void_p]
arpirobot.IRReflectorModule_getDigitalValue.restype = ctypes.c_bool

arpirobot.IRReflectorModule_getAnalogValue.argtypes = [ctypes.c_void_p]
arpirobot.IRReflectorModule_getAnalogValue.restype = ctypes.c_int


################################################################################
# OldAdafruit9Dof Bridge
################################################################################

arpirobot.OldAdafruit9Dof_create.argtypes = [ctypes.c_bool, ctypes.c_int]
arpirobot.OldAdafruit9Dof_create.restype = ctypes.c_void_p

arpirobot.OldAdafruit9Dof_destroy.argtypes = [ctypes.c_void_p]
arpirobot.OldAdafruit9Dof_destroy.restype = None

arpirobot.OldAdafruit9Dof_getGyroX.argtypes = [ctypes.c_void_p]
arpirobot.OldAdafruit9Dof_getGyroX.restype = ctypes.c_double

arpirobot.OldAdafruit9Dof_getGyroY.argtypes = [ctypes.c_void_p]
arpirobot.OldAdafruit9Dof_getGyroY.restype = ctypes.c_double

arpirobot.OldAdafruit9Dof_getGyroZ.argtypes = [ctypes.c_void_p]
arpirobot.OldAdafruit9Dof_getGyroZ.restype = ctypes.c_double

arpirobot.OldAdafruit9Dof_getAccelX.argtypes = [ctypes.c_void_p]
arpirobot.OldAdafruit9Dof_getAccelX.restype = ctypes.c_double

arpirobot.OldAdafruit9Dof_getAccelY.argtypes = [ctypes.c_void_p]
arpirobot.OldAdafruit9Dof_getAccelY.restype = ctypes.c_double

arpirobot.OldAdafruit9Dof_getAccelZ.argtypes = [ctypes.c_void_p]
arpirobot.OldAdafruit9Dof_getAccelZ.restype = ctypes.c_double

arpirobot.OldAdafruit9Dof_setGyroX.argtypes = [ctypes.c_void_p, ctypes.c_double]
arpirobot.OldAdafruit9Dof_setGyroX.restype = None

arpirobot.OldAdafruit9Dof_setGyroY.argtypes = [ctypes.c_void_p, ctypes.c_double]
arpirobot.OldAdafruit9Dof_setGyroY.restype = None

arpirobot.OldAdafruit9Dof_setGyroZ.argtypes = [ctypes.c_void_p, ctypes.c_double]
arpirobot.OldAdafruit9Dof_setGyroZ.restype = None


################################################################################
# NxpAdafruit9Dof Bridge
################################################################################

arpirobot.NxpAdafruit9Dof_create.argtypes = [ctypes.c_bool, ctypes.c_int]
arpirobot.NxpAdafruit9Dof_create.restype = ctypes.c_void_p

arpirobot.NxpAdafruit9Dof_destroy.argtypes = [ctypes.c_void_p]
arpirobot.NxpAdafruit9Dof_destroy.restype = None

arpirobot.NxpAdafruit9Dof_getGyroX.argtypes = [ctypes.c_void_p]
arpirobot.NxpAdafruit9Dof_getGyroX.restype = ctypes.c_double

arpirobot.NxpAdafruit9Dof_getGyroY.argtypes = [ctypes.c_void_p]
arpirobot.NxpAdafruit9Dof_getGyroY.restype = ctypes.c_double

arpirobot.NxpAdafruit9Dof_getGyroZ.argtypes = [ctypes.c_void_p]
arpirobot.NxpAdafruit9Dof_getGyroZ.restype = ctypes.c_double

arpirobot.NxpAdafruit9Dof_getAccelX.argtypes = [ctypes.c_void_p]
arpirobot.NxpAdafruit9Dof_getAccelX.restype = ctypes.c_double

arpirobot.NxpAdafruit9Dof_getAccelY.argtypes = [ctypes.c_void_p]
arpirobot.NxpAdafruit9Dof_getAccelY.restype = ctypes.c_double

arpirobot.NxpAdafruit9Dof_getAccelZ.argtypes = [ctypes.c_void_p]
arpirobot.NxpAdafruit9Dof_getAccelZ.restype = ctypes.c_double

arpirobot.NxpAdafruit9Dof_setGyroX.argtypes = [ctypes.c_void_p, ctypes.c_double]
arpirobot.NxpAdafruit9Dof_setGyroX.restype = None

arpirobot.NxpAdafruit9Dof_setGyroY.argtypes = [ctypes.c_void_p, ctypes.c_double]
arpirobot.NxpAdafruit9Dof_setGyroY.restype = None

arpirobot.NxpAdafruit9Dof_setGyroZ.argtypes = [ctypes.c_void_p, ctypes.c_double]
arpirobot.NxpAdafruit9Dof_setGyroZ.restype = None


################################################################################
# INA260PowerSensor Bridge
################################################################################

arpirobot.INA260PowerSensor_create.argtypes = []
arpirobot.INA260PowerSensor_create.restype = ctypes.c_void_p

arpirobot.INA260PowerSensor_destroy.argtypes = [ctypes.c_void_p]
arpirobot.INA260PowerSensor_destroy.restype = None

arpirobot.INA260PowerSensor_getCurrent.argtypes = [ctypes.c_void_p]
arpirobot.INA260PowerSensor_getCurrent.restype = ctypes.c_double

arpirobot.INA260PowerSensor_getVoltage.argtypes = [ctypes.c_void_p]
arpirobot.INA260PowerSensor_getVoltage.restype = ctypes.c_double

arpirobot.INA260PowerSensor_getPower.argtypes = [ctypes.c_void_p]
arpirobot.INA260PowerSensor_getPower.restype = ctypes.c_double

arpirobot.INA260PowerSensor_makeMainVmon.argtypes = [ctypes.c_void_p]
arpirobot.INA260PowerSensor_makeMainVmon.restype = None
