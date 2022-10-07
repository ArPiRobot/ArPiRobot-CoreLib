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

import ctypes
import platform

if platform.system() == "Windows":
    from ctypes.util import find_library
    arpirobot = ctypes.CDLL(find_library("libarpirobot-core.dll"))
else:
    arpirobot = ctypes.CDLL("libarpirobot-core.so")

# NOTE: If returning dynamically allocated string restype should be ptr (ctypes.c_void_p)
#       Using c_char_p will convert to python bytes then can't call free


################################################################################
# General / Helper
################################################################################

arpirobot.freeString.argtypes = [ctypes.c_char_p]
arpirobot.freeString.restype = None

arpirobot.copyToNewPointerArray.argtypes = [ctypes.c_void_p, ctypes.c_size_t]
arpirobot.copyToNewPointerArray.restype = ctypes.c_void_p

################################################################################
# BaseRobot Bridge
################################################################################

arpirobot.BaseRobot_create.argtypes = [ctypes.c_void_p, 
    ctypes.c_void_p, 
    ctypes.c_void_p, 
    ctypes.c_void_p, 
    ctypes.c_void_p, 
    ctypes.c_void_p]
arpirobot.BaseRobot_create.restype = ctypes.c_void_p

arpirobot.BaseRobot_destroy.argtypes = [ctypes.c_void_p]
arpirobot.BaseRobot_destroy.restype = None

arpirobot.BaseRobot_start.argtypes = [ctypes.c_void_p]
arpirobot.BaseRobot_start.restype = None

arpirobot.BaseRobot_feedWatchdog.argtypes = [ctypes.c_void_p]
arpirobot.BaseRobot_feedWatchdog.restype = None


################################################################################
# RobotProfile Bridge
################################################################################

arpirobot.RobotProfile_setMainSchedulerThreads.argtypes = [ctypes.c_int]
arpirobot.RobotProfile_setMainSchedulerThreads.restype = None

arpirobot.RobotProfile_getMainSchedulerThreads.argtypes = []
arpirobot.RobotProfile_getMainSchedulerThreads.restype = ctypes.c_int

arpirobot.RobotProfile_setPeriodicFunctionRate.argtypes = [ctypes.c_int]
arpirobot.RobotProfile_setPeriodicFunctionRate.restype = None

arpirobot.RobotProfile_getPeriodicFunctionRate.argtypes = []
arpirobot.RobotProfile_getPeriodicFunctionRate.restype = ctypes.c_int

arpirobot.RobotProfile_setMaxGamepadDataAge.argtypes = [ctypes.c_int]
arpirobot.RobotProfile_setMaxGamepadDataAge.restype = None

arpirobot.RobotProfile_getMaxGamepadDataAge.argtypes = []
arpirobot.RobotProfile_getMaxGamepadDataAge.restype = ctypes.c_int

arpirobot.RobotProfile_setActionFunctionPeriod.argtypes = [ctypes.c_int]
arpirobot.RobotProfile_setActionFunctionPeriod.restype = None

arpirobot.RobotProfile_getActionFunctionPeriod.argtypes = []
arpirobot.RobotProfile_getActionFunctionPeriod.restype = ctypes.c_int

arpirobot.RobotProfile_setDeviceWatchdogDur.argtypes = [ctypes.c_int]
arpirobot.RobotProfile_setDeviceWatchdogDur.restype = None

arpirobot.RobotProfile_getDeviceWatchdogDur.argtypes = []
arpirobot.RobotProfile_getDeviceWatchdogDur.restype = ctypes.c_int

arpirobot.RobotProfile_setIoProvider.argtypes = [ctypes.c_char_p]
arpirobot.RobotProfile_setIoProvider.restype = None

arpirobot.RobotProfile_getIoProvider.argtypes = []
arpirobot.RobotProfile_getIoProvider.restype = ctypes.c_void_p

################################################################################
# MainVMon Bridge
################################################################################

arpirobot.MainVMon_makeMainVmon.argtypes = [ctypes.c_void_p]
arpirobot.MainVMon_makeMainVmon.restype = None

################################################################################
# NetworkTable Bridge
################################################################################

arpirobot.NetworkTable_set.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
arpirobot.NetworkTable_set.restype = None

arpirobot.NetworkTable_get.argtypes = [ctypes.c_char_p]
arpirobot.NetworkTable_get.restype = ctypes.c_void_p 

arpirobot.NetworkTable_has.argtypes = [ctypes.c_char_p]
arpirobot.NetworkTable_has.restype = ctypes.c_bool

arpirobot.NetworkTable_changed.argtypes = [ctypes.c_char_p]
arpirobot.NetworkTable_changed.restype = ctypes.c_bool

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

arpirobot.AdafruitMotorHatMotor_create.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_bool]
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

arpirobot.Action_create.argtypes = [ctypes.c_int32, ctypes.c_void_p, ctypes.c_void_p, ctypes.c_void_p, ctypes.c_void_p, ctypes.c_void_p]
arpirobot.Action_create.restype = ctypes.c_void_p

arpirobot.Action_destroy.argtypes = [ctypes.c_void_p]
arpirobot.Action_destroy.restype = None

arpirobot.Action_isRunning.argtypes = [ctypes.c_void_p]
arpirobot.Action_isRunning.restype = ctypes.c_bool

arpirobot.Action_setProcessPeriodMs.argtypes = [ctypes.c_void_p, ctypes.c_int32]
arpirobot.Action_setProcessPeriodMs.restype = None

arpirobot.Action_getProcessPeriodMs.argtypes = [ctypes.c_void_p]
arpirobot.Action_getProcessPeriodMs.restype = ctypes.c_int32

################################################################################
# ActionManager Bridge
################################################################################

arpirobot.ActionManager_startAction.argtypes = [ctypes.c_void_p]
arpirobot.ActionManager_startAction.restype = ctypes.c_bool

arpirobot.ActionManager_stopAction.argtypes = [ctypes.c_void_p]
arpirobot.ActionManager_stopAction.restype = ctypes.c_bool

arpirobot.ActionManager_addTrigger.argtypes = [ctypes.c_void_p]
arpirobot.ActionManager_addTrigger.restype = None

arpirobot.ActionManager_removeTrigger.argtypes = [ctypes.c_void_p]
arpirobot.ActionManager_removeTrigger.restype = None


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
arpirobot.SingleEncoder_getPosition.restype = ctypes.c_int32

arpirobot.SingleEncoder_setPosition.argtypes = [ctypes.c_void_p, ctypes.c_int32]
arpirobot.SingleEncoder_setPosition.restype = None

arpirobot.SingleEncoder_getVelocity.argtypes = [ctypes.c_void_p]
arpirobot.SingleEncoder_getVelocity.restype = ctypes.c_float


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

arpirobot.NxpAdafruit9Dof_calibrate.argtypes = [ctypes.c_void_p, ctypes.c_int]
arpirobot.NxpAdafruit9Dof_calibrate.restype = None

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
# Mpu6050Imu Bridge
################################################################################

arpirobot.Mpu6050Imu_create.argtypes = [ctypes.c_bool, ctypes.c_int]
arpirobot.Mpu6050Imu_create.restype = ctypes.c_void_p

arpirobot.Mpu6050Imu_destroy.argtypes = [ctypes.c_void_p]
arpirobot.Mpu6050Imu_destroy.restype = None

arpirobot.Mpu6050Imu_calibrate.argtypes = [ctypes.c_void_p, ctypes.c_int]
arpirobot.Mpu6050Imu_calibrate.restype = None

arpirobot.Mpu6050Imu_getGyroX.argtypes = [ctypes.c_void_p]
arpirobot.Mpu6050Imu_getGyroX.restype = ctypes.c_double

arpirobot.Mpu6050Imu_getGyroY.argtypes = [ctypes.c_void_p]
arpirobot.Mpu6050Imu_getGyroY.restype = ctypes.c_double

arpirobot.Mpu6050Imu_getGyroZ.argtypes = [ctypes.c_void_p]
arpirobot.Mpu6050Imu_getGyroZ.restype = ctypes.c_double

arpirobot.Mpu6050Imu_getAccelX.argtypes = [ctypes.c_void_p]
arpirobot.Mpu6050Imu_getAccelX.restype = ctypes.c_double

arpirobot.Mpu6050Imu_getAccelY.argtypes = [ctypes.c_void_p]
arpirobot.Mpu6050Imu_getAccelY.restype = ctypes.c_double

arpirobot.Mpu6050Imu_getAccelZ.argtypes = [ctypes.c_void_p]
arpirobot.Mpu6050Imu_getAccelZ.restype = ctypes.c_double

arpirobot.Mpu6050Imu_setGyroX.argtypes = [ctypes.c_void_p, ctypes.c_double]
arpirobot.Mpu6050Imu_setGyroX.restype = None

arpirobot.Mpu6050Imu_setGyroY.argtypes = [ctypes.c_void_p, ctypes.c_double]
arpirobot.Mpu6050Imu_setGyroY.restype = None

arpirobot.Mpu6050Imu_setGyroZ.argtypes = [ctypes.c_void_p, ctypes.c_double]
arpirobot.Mpu6050Imu_setGyroZ.restype = None


################################################################################
# QuadEncoder Bridge
################################################################################

arpirobot.QuadEncoder_create.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_bool, ctypes.c_bool, ctypes.c_int]
arpirobot.QuadEncoder_create.restype = ctypes.c_void_p

arpirobot.QuadEncoder_destroy.argtypes = [ctypes.c_void_p]
arpirobot.QuadEncoder_destroy.restype = None

arpirobot.QuadEncoder_getPosition.argtypes = [ctypes.c_void_p]
arpirobot.QuadEncoder_getPosition.restype = ctypes.c_int32

arpirobot.QuadEncoder_setPosition.argtypes = [ctypes.c_void_p, ctypes.c_int32]
arpirobot.QuadEncoder_setPosition.restype = None

arpirobot.QuadEncoder_getVelocity.argtypes = [ctypes.c_void_p]
arpirobot.QuadEncoder_getVelocity.restype = ctypes.c_float


################################################################################
# INA260PowerSensor Bridge
################################################################################

arpirobot.INA260PowerSensor_create.argtypes = [ctypes.c_int]
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


################################################################################
# StatusLED Bridge
################################################################################

arpirobot.StatusLED_create.argtypes = [ctypes.c_uint]
arpirobot.StatusLED_create.restype = ctypes.c_void_p

arpirobot.StatusLED_destroy.argtypes = [ctypes.c_void_p]
arpirobot.StatusLED_destroy.restype = None

################################################################################
# GPIOPin Bridge
################################################################################

arpirobot.GPIOPin_create.argtypes = [ctypes.c_uint]
arpirobot.GPIOPin_create.restype = ctypes.c_void_p

arpirobot.GPIOPin_destroy.argtypes = [ctypes.c_void_p]
arpirobot.GPIOPin_destroy.restype = None

arpirobot.GPIOPin_setMode.argtypes = [ctypes.c_void_p, ctypes.c_int]
arpirobot.GPIOPin_setMode.restype = None

arpirobot.GPIOPin_setLevel.argtypes = [ctypes.c_void_p, ctypes.c_int]
arpirobot.GPIOPin_setLevel.restype = None

arpirobot.GPIOPin_getLevel.argtypes = [ctypes.c_void_p]
arpirobot.GPIOPin_getLevel.restype = ctypes.c_int

arpirobot.GPIOPin_setPwmValue.argtypes = [ctypes.c_void_p, ctypes.c_uint8]
arpirobot.GPIOPin_setPwmValue.restype = None

arpirobot.GPIOPin_getPwmValue.argtypes = [ctypes.c_void_p]
arpirobot.GPIOPin_getPwmValue.restype = ctypes.c_uint8

arpirobot.GPIOPin_setPwmFrequency.argtypes = [ctypes.c_void_p, ctypes.c_uint]
arpirobot.GPIOPin_setPwmFrequency.restype = None

arpirobot.GPIOPin_getPwmFrequency.argtypes = [ctypes.c_void_p]
arpirobot.GPIOPin_getPwmFrequency.restype = ctypes.c_uint


################################################################################
# AudioManager Bridge
################################################################################

arpirobot.AudioManager_getPlaybackDevicesCount.argtypes = []
arpirobot.AudioManager_getPlaybackDevicesCount.restype = ctypes.c_size_t

arpirobot.AudioManager_getPlaybackDevice.argtypes = [ctypes.c_size_t, ctypes.POINTER(ctypes.c_uint32), ctypes.POINTER(ctypes.c_char_p), ctypes.POINTER(ctypes.c_bool), ctypes.POINTER(ctypes.c_uint8)]
arpirobot.AudioManager_getPlaybackDevice.restype = None

arpirobot.AudioManager_playSound.argtypes = [ctypes.c_char_p]
arpirobot.AudioManager_playSound.restype = ctypes.c_int

arpirobot.AudioManager_playSoundWithDevice.argtypes = [ctypes.c_char_p, ctypes.c_uint32, ctypes.c_char_p, ctypes.c_bool, ctypes.c_uint8]
arpirobot.AudioManager_playSoundWithDevice.restype = ctypes.c_int

arpirobot.AudioManager_stopJob.argtypes = [ctypes.c_int]
arpirobot.AudioManager_stopJob.restype = None


################################################################################
# PID Bridge
################################################################################

arpirobot.PID_create.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double]
arpirobot.PID_create.restype = ctypes.c_void_p

arpirobot.PID_destroy.argtypes = [ctypes.c_void_p]
arpirobot.PID_destroy.restype = None

arpirobot.PID_getKp.argtypes = [ctypes.c_void_p]
arpirobot.PID_getKp.restype = ctypes.c_double

arpirobot.PID_setKp.argtypes = [ctypes.c_void_p, ctypes.c_double]
arpirobot.PID_setKp.restype = None

arpirobot.PID_getKi.argtypes = [ctypes.c_void_p]
arpirobot.PID_getKi.restype = ctypes.c_double

arpirobot.PID_setKi.argtypes = [ctypes.c_void_p, ctypes.c_double]
arpirobot.PID_setKi.restype = None

arpirobot.PID_getKd.argtypes = [ctypes.c_void_p]
arpirobot.PID_getKd.restype = ctypes.c_double

arpirobot.PID_setKd.argtypes = [ctypes.c_void_p, ctypes.c_double]
arpirobot.PID_setKd.restype = None

arpirobot.PID_getKf.argtypes = [ctypes.c_void_p]
arpirobot.PID_getKf.restype = ctypes.c_double

arpirobot.PID_setKf.argtypes = [ctypes.c_void_p, ctypes.c_double]
arpirobot.PID_setKf.restype = None

arpirobot.PID_getMin.argtypes = [ctypes.c_void_p]
arpirobot.PID_getMin.restype = ctypes.c_double

arpirobot.PID_setMin.argtypes = [ctypes.c_void_p, ctypes.c_double]
arpirobot.PID_setMin.restype = None

arpirobot.PID_getMax.argtypes = [ctypes.c_void_p]
arpirobot.PID_getMax.restype = ctypes.c_double

arpirobot.PID_setMax.argtypes = [ctypes.c_void_p, ctypes.c_double]
arpirobot.PID_setMax.restype = None

arpirobot.PID_getSetpoint.argtypes = [ctypes.c_void_p]
arpirobot.PID_getSetpoint.restype = ctypes.c_double

arpirobot.PID_setSetpoint.argtypes = [ctypes.c_void_p, ctypes.c_double]
arpirobot.PID_setSetpoint.restype = None

arpirobot.PID_reset.argtypes = [ctypes.c_void_p]
arpirobot.PID_reset.restype = None

arpirobot.PID_getOutput.argtypes = [ctypes.c_void_p, ctypes.c_double]
arpirobot.PID_getOutput.restype = ctypes.c_double

