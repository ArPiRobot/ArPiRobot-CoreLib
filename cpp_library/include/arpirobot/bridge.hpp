/*
 * Copyright 2021 Marcus Behel
 *
 * This file is part of ArPiRobot-CoreLib.
 * 
 * ArPiRobot-CoreLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ArPiRobot-CoreLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with ArPiRobot-CoreLib.  If not, see <https://www.gnu.org/licenses/>. 
 */

#pragma once

#include <arpirobot/core/robot/BaseRobot.hpp>
#include <arpirobot/core/robot/RobotProfile.hpp>

#include <arpirobot/core/log/Logger.hpp>

#include <arpirobot/devices/gamepad/Gamepad.hpp>
#include <arpirobot/devices/gamepad/ButtonPressedTrigger.hpp>
#include <arpirobot/devices/gamepad/ButtonReleasedTrigger.hpp>

#include <arpirobot/core/network/NetworkTable.hpp>

#include <arpirobot/devices/adafruitmotorhat/AdafruitMotorHatMotor.hpp>

#include <arpirobot/devices/l298n/L298NModule.hpp>
#include <arpirobot/devices/l298n/L298NMotor.hpp>

#include <arpirobot/devices/tb6612/TB6612Module.hpp>
#include <arpirobot/devices/tb6612/TB6612Motor.hpp>

#include <arpirobot/devices/drv8833/DRV8833Module.hpp>
#include <arpirobot/devices/drv8833/DRV8833Motor.hpp>

#include <arpirobot/core/drive/ArcadeDriveHelper.hpp>
#include <arpirobot/core/drive/BaseAxisTransform.hpp>
#include <arpirobot/core/drive/CubicAxisTransform.hpp>
#include <arpirobot/core/drive/SquareRootAxisTransform.hpp>
#include <arpirobot/core/drive/TankDriveHelper.hpp>


#include <arpirobot/core/device/BaseDevice.hpp>
#include <arpirobot/core/device/MotorController.hpp>

#include <arpirobot/core/action/Action.hpp>
#include <arpirobot/core/action/ActionManager.hpp>
#include <arpirobot/core/action/ActionSeries.hpp>
#include <arpirobot/core/action/BaseActionTrigger.hpp>

#include <arpirobot/arduino/iface/BaseArduinoInterface.hpp>
#include <arpirobot/arduino/iface/ArduinoUartInterface.hpp>

#include <arpirobot/arduino/device/ArduinoDevice.hpp>

#include <arpirobot/arduino/sensor/IRReflectorModule.hpp>
#include <arpirobot/arduino/sensor/Mpu6050Imu.hpp>
#include <arpirobot/arduino/sensor/NxpAdafruit9Dof.hpp>
#include <arpirobot/arduino/sensor/OldAdafruit9Dof.hpp>
#include <arpirobot/arduino/sensor/SingleEncoder.hpp>
#include <arpirobot/arduino/sensor/Ultrasonic4Pin.hpp>
#include <arpirobot/arduino/sensor/VoltageMonitor.hpp>

#include <arpirobot/devices/ina260/INA260PowerSensor.hpp>

#include <arpirobot/devices/gpio/StatusLED.hpp>
#include <arpirobot/devices/gpio/GPIOPin.hpp>

#include <arpirobot/core/control/PID.hpp>

#include <string>
#include <memory>
#include <vector>

using namespace arpirobot;

#define BRIDGE_FUNC extern "C"

////////////////////////////////////////////////////////////////////////////////
/// General/Helper
////////////////////////////////////////////////////////////////////////////////

// Use to create a string that can be returned from one of these functions.
BRIDGE_FUNC char *returnableString(std::string str);

// Once the other language copies data out of the returned string free the memory
// THIS MUST BE MANUALLY CALLED BY THE OTHER LANGUAGE's BRIDGE
BRIDGE_FUNC void freeString(char *str);


////////////////////////////////////////////////////////////////////////////////
/// BaseRobot Bridge
////////////////////////////////////////////////////////////////////////////////

// Wrapper to implement abstract functions using callback pointers
class BridgeBaseRobot : public BaseRobot{
public:
    BridgeBaseRobot(void (*robotStartedPtr)(void), 
                        void (*robotEnabledPtr)(void), 
                        void (*robotDisabledPtr)(void), 
                        void (*enabledPeriodicPtr)(void), 
                        void (*disabledPeriodicPtr)(void), 
                        void (*periodicPtr)(void),
                        void (*destoryedPtr)(void),
                        RobotProfile profile);
    
    ~BridgeBaseRobot();

    void robotStarted();
    void robotEnabled();
    void robotDisabled();
    void enabledPeriodic();
    void disabledPeriodic();
    void periodic();

private:
    void (*robotStartedPtr)(void);
    void (*robotEnabledPtr)(void); 
    void (*robotDisabledPtr)(void); 
    void (*enabledPeriodicPtr)(void); 
    void (*disabledPeriodicPtr)(void); 
    void (*periodicPtr)(void);
    void (*destroyedPtr)(void);
};

BRIDGE_FUNC BaseRobot* BaseRobot_create(void (*robotStarted)(void), 
                        void (*robotEnabled)(void), 
                        void (*robotDisabled)(void), 
                        void (*enabledPeriodic)(void), 
                        void (*disabledPeriodic)(void), 
                        void (*periodic)(void),
                        void (*destroyed)(void),
                        int mainSchedulerThreads,
                        int periodicFunctionRate,
                        int maxGamepadDataAge,
                        int actionFunctionPeriod);

BRIDGE_FUNC void BaseRobot_destroy(BaseRobot *robot);

BRIDGE_FUNC void BaseRobot_start(BaseRobot *robot, const char *ioProvider);

BRIDGE_FUNC void BaseRobot_feedWatchdog(BaseRobot *robot);


////////////////////////////////////////////////////////////////////////////////
/// MainVMon Bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC void MainVMon_makeMainVmon(MainVmon *vmon);

////////////////////////////////////////////////////////////////////////////////
/// NetworkTable Bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC void NetworkTable_set(const char *key, const char *value);

BRIDGE_FUNC char *NetworkTable_get(const char *key);

BRIDGE_FUNC bool NetworkTable_has(const char *key);

BRIDGE_FUNC bool NetworkTable_changed(const char *key);

////////////////////////////////////////////////////////////////////////////////
/// Logger Bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC void Logger_logDebug(const char *message);

BRIDGE_FUNC void Logger_logInfo(const char *message);

BRIDGE_FUNC void Logger_logWarning(const char *message);

BRIDGE_FUNC void Logger_logError(const char *message);

BRIDGE_FUNC void Logger_logDebugFrom(const char *source, const char *message);

BRIDGE_FUNC void Logger_logInfoFrom(const char *source, const char *message);

BRIDGE_FUNC void Logger_logWarningFrom(const char *source, const char *message);

BRIDGE_FUNC void Logger_logErrorFrom(const char *source, const char *message);

BRIDGE_FUNC void Logger_logNewline();


////////////////////////////////////////////////////////////////////////////////
/// BaseDevice bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC char *BaseDevice_getDeviceName(BaseDevice *device);


////////////////////////////////////////////////////////////////////////////////
/// Gamepad bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC Gamepad *Gamepad_create(int controllerNum);

BRIDGE_FUNC void Gamepad_destroy(Gamepad *gamepad);

BRIDGE_FUNC int Gamepad_getControllerNum(Gamepad *gamepad);

BRIDGE_FUNC double Gamepad_getAxis(Gamepad *gamepad, int axisNum, double deadband);

BRIDGE_FUNC bool Gamepad_getButton(Gamepad *gamepad, int buttonNum);

BRIDGE_FUNC int Gamepad_getDpad(Gamepad *gamepad, int dpadNum);

BRIDGE_FUNC void Gamepad_setAxisTransform(Gamepad *gamepad, int axisNum, BaseAxisTransform *transform);

BRIDGE_FUNC void Gamepad_clearAxisTransform(Gamepad *gamepad, int axisNum);


////////////////////////////////////////////////////////////////////////////////
/// ButtonPressedTrigger bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC ButtonPressedTrigger *ButtonPressedTrigger_create(Gamepad *gamepad, int buttonNum, 
    Action *targetAction, bool doRestart);

BRIDGE_FUNC void ButtonPressedTrigger_destroy(ButtonPressedTrigger *trigger);


////////////////////////////////////////////////////////////////////////////////
/// ButtonReleasedTrigger bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC ButtonReleasedTrigger *ButtonReleasedTrigger_create(Gamepad *gamepad, int buttonNum, 
    Action *targetAction, bool doRestart);

BRIDGE_FUNC void ButtonReleasedTrigger_destroy(ButtonReleasedTrigger *trigger);


////////////////////////////////////////////////////////////////////////////////
/// MotorController bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC bool MotorController_isInverted(MotorController *motor);

BRIDGE_FUNC void MotorController_setInverted(MotorController *motor, bool inverted);

BRIDGE_FUNC bool MotorController_isBrakeMode(MotorController *motor);

BRIDGE_FUNC void MotorController_setBrakeMode(MotorController *motor, bool brakeMode);

BRIDGE_FUNC void MotorController_setSpeed(MotorController *motor, double speed);

BRIDGE_FUNC double MotorController_getSpeed(MotorController *motor);


////////////////////////////////////////////////////////////////////////////////
/// AdafruitMotorHatMotor bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC AdafruitMotorHatMotor *AdafruitMotorHatMotor_create(int motorNum, int address, bool remapNumbers);

BRIDGE_FUNC void AdafruitMotorHatMotor_destroy(AdafruitMotorHatMotor *motor);


////////////////////////////////////////////////////////////////////////////////
/// L298Motor bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC L298NMotor *L298NMotor_create(int in1Pin, int in2Pin, int pwmPin);

BRIDGE_FUNC void L298NMotor_destroy(L298NMotor *motor);


////////////////////////////////////////////////////////////////////////////////
/// TB6612Motor bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC TB6612Motor *TB6612Motor_create(int in1Pin, int in2Pin, int pwmPin);

BRIDGE_FUNC void TB6612Motor_destroy(TB6612Motor *motor);


////////////////////////////////////////////////////////////////////////////////
/// DRV8833Motor bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC DRV8833Motor *DRV8833Motor_create(int in1Pin, int in2Pin, int slpPin);

BRIDGE_FUNC void DRV8833Motor_destroy(DRV8833Motor *motor);


////////////////////////////////////////////////////////////////////////////////
/// ArcadeDriveHelper bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC ArcadeDriveHelper *ArcadeDriveHelper_create(MotorController **leftMotors, size_t leftMotorCount, 
    MotorController **rightMotors, size_t rightMotorCount);

BRIDGE_FUNC void ArcadeDriveHelper_destroy(ArcadeDriveHelper *helper);

BRIDGE_FUNC void ArcadeDriveHelper_updateSpeed(ArcadeDriveHelper *helper, double speed);

BRIDGE_FUNC void ArcadeDriveHelper_updateRotation(ArcadeDriveHelper *helper, double rotation);

BRIDGE_FUNC void ArcadeDriveHelper_update(ArcadeDriveHelper *helper, double speed, double rotation);


////////////////////////////////////////////////////////////////////////////////
/// TankDriveHelper bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC TankDriveHelper *TankDriveHelper_create(MotorController **leftMotors, size_t leftMotorCount, 
    MotorController **rightMotors, size_t rightMotorCount);

BRIDGE_FUNC void TankDriveHelper_destroy(TankDriveHelper *helper);

BRIDGE_FUNC void TankDriveHelper_updateLeftSpeed(TankDriveHelper *helper, double newLeftSpeed);

BRIDGE_FUNC void TankDriveHelper_updateRightSpeed(TankDriveHelper *helper, double newRightSpeed);

BRIDGE_FUNC void TankDriveHelper_update(TankDriveHelper *helper, double newLeftSpeed, double newRightSpeed);


////////////////////////////////////////////////////////////////////////////////
/// BaseAxisTransform bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC double BaseAxisTransform_applyTransform(BaseAxisTransform *transform, double rawAxisValue);


////////////////////////////////////////////////////////////////////////////////
/// SquareRootAxisTransform bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC SquareRootAxisTransform *SquareRootAxisTransform_create();

BRIDGE_FUNC void SquareRootAxisTransform_destroy(SquareRootAxisTransform *transform);


////////////////////////////////////////////////////////////////////////////////
/// CubicAxisTransform bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC CubicAxisTransform *CubicAxisTransform_create(double minPower, double midPower);

BRIDGE_FUNC void CubicAxisTransform_destroy(CubicAxisTransform *transform);


////////////////////////////////////////////////////////////////////////////////
/// Action Bridge 
////////////////////////////////////////////////////////////////////////////////


class BridgeAction : public Action{
public:
    BridgeAction(void (*beginPtr)(void),
        void (*processPtr)(void),
        void (*finishPtr)(bool),
        bool (*shouldContinuePtr)(void),
        void (*destroyedPtr)(void));

    ~BridgeAction();

    void begin();
    void process();
    void finish(bool interrupted);
    bool shouldContinue();

private:
    void (*beginPtr)(void);
    void (*processPtr)(void);
    void (*finishPtr)(bool);
    bool (*shouldContinuePtr)(void);
    void (*destroyedPtr)(void);
};

BRIDGE_FUNC Action *Action_create(void (*beginPtr)(void),
    void (*processPtr)(void),
    void (*finishPtr)(bool),
    bool (*shouldContinuePtr)(void),
    void (*destroyedPtr)(void));

BRIDGE_FUNC void Action_destroy(Action *action);

BRIDGE_FUNC void Action_lockDevices(Action *action, BaseDevice **devices, size_t deviceCount);

BRIDGE_FUNC void Action_lockDevice(Action *action, BaseDevice *device);

BRIDGE_FUNC bool Action_isRunning(Action *action);

BRIDGE_FUNC void Action_setProcessPeriodMs(Action *action, int32_t processPeriodMs);

////////////////////////////////////////////////////////////////////////////////
/// ActionManager Bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC bool ActionManager_startAction(Action *action);

BRIDGE_FUNC bool ActionManager_stopAction(Action *action);

BRIDGE_FUNC void ActionManager_addTrigger(BaseActionTrigger *trigger);

BRIDGE_FUNC void ActionManager_removeTrigger(BaseActionTrigger *trigger);


////////////////////////////////////////////////////////////////////////////////
/// ActionSeries Bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC ActionSeries *ActionSeries_create(Action **actions, size_t actionCount, Action* finishAction);

BRIDGE_FUNC void ActionSeries_destroy(ActionSeries *actionSeries);


////////////////////////////////////////////////////////////////////////////////
/// BaseArduinoInterface bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC void BaseArduinoInterface_begin(BaseArduinoInterface *arduino);

BRIDGE_FUNC void BaseArduinoInterface_addDevice(BaseArduinoInterface *arduino, ArduinoDevice *device);

BRIDGE_FUNC bool BaseArduinoInterface_isReady(BaseArduinoInterface *arduino);


////////////////////////////////////////////////////////////////////////////////
/// ArduinoUartInterface bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC ArduinoUartInterface *ArduinoUartInterface_create(const char *tty, int baud);

BRIDGE_FUNC void ArduinoUartInterface_destroy(ArduinoUartInterface *interface);


////////////////////////////////////////////////////////////////////////////////
/// ArduinoDevice bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC char *ArduinoDevice_getDeviceName(ArduinoDevice *device);


////////////////////////////////////////////////////////////////////////////////
/// VoltageMonitor bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC VoltageMonitor *VoltageMonitor_create(const char *pin, double vboard, int r1, 
    int r2, bool createDevice, int deviceId);

BRIDGE_FUNC void VoltageMonitor_destroy(VoltageMonitor *vmon);

BRIDGE_FUNC double VoltageMonitor_getVoltage(VoltageMonitor *vmon);


////////////////////////////////////////////////////////////////////////////////
/// Ultrasonic4Pin bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC Ultrasonic4Pin *Ultrasonic4Pin_create(const char *triggerPin, const char *echoPin, 
    bool createDevice, int deviceId);

BRIDGE_FUNC void Ultrasonic4Pin_destroy(Ultrasonic4Pin *usonic);

BRIDGE_FUNC int Ultrasonic4Pin_getDistance(Ultrasonic4Pin *usonic);


////////////////////////////////////////////////////////////////////////////////
/// SingleEncode bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC SingleEncoder *SingleEncoder_create(const char *pin, bool useInternalPullup, 
    bool createDevice, int deviceId);

BRIDGE_FUNC void SingleEncoder_destroy(SingleEncoder *enc);

BRIDGE_FUNC int SingleEncoder_getPosition(SingleEncoder *enc);

BRIDGE_FUNC void SingleEncoder_setPosition(SingleEncoder *enc, int newPosition);


////////////////////////////////////////////////////////////////////////////////
/// IRReflectorModule bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC IRReflectorModule *IRReflectorModule_create(const char *digitalPin, const char *analogPin, 
    bool createDevice, int deviceId);

BRIDGE_FUNC void IRReflectorModule_destroy(IRReflectorModule *ir);

BRIDGE_FUNC bool IRReflectorModule_getDigitalValue(IRReflectorModule *ir);

BRIDGE_FUNC int IRReflectorModule_getAnalogValue(IRReflectorModule *ir);


////////////////////////////////////////////////////////////////////////////////
/// OldAdafruit9Dof bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC OldAdafruit9Dof *OldAdafruit9Dof_create(bool createDevice, int deviceId);

BRIDGE_FUNC void OldAdafruit9Dof_destroy(OldAdafruit9Dof *imu);

BRIDGE_FUNC double OldAdafruit9Dof_getGyroX(OldAdafruit9Dof *imu);

BRIDGE_FUNC double OldAdafruit9Dof_getGyroY(OldAdafruit9Dof *imu);

BRIDGE_FUNC double OldAdafruit9Dof_getGyroZ(OldAdafruit9Dof *imu);

BRIDGE_FUNC double OldAdafruit9Dof_getAccelX(OldAdafruit9Dof *imu);

BRIDGE_FUNC double OldAdafruit9Dof_getAccelY(OldAdafruit9Dof *imu);

BRIDGE_FUNC double OldAdafruit9Dof_getAccelZ(OldAdafruit9Dof *imu);

BRIDGE_FUNC void OldAdafruit9Dof_setGyroX(OldAdafruit9Dof *imu, double newGyroX);

BRIDGE_FUNC void OldAdafruit9Dof_setGyroY(OldAdafruit9Dof *imu, double newGyroY);

BRIDGE_FUNC void OldAdafruit9Dof_setGyroZ(OldAdafruit9Dof *imu, double newGyroZ);


////////////////////////////////////////////////////////////////////////////////
/// NxpAdafruit9Dof bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC NxpAdafruit9Dof *NxpAdafruit9Dof_create(bool createDevice, int deviceId);

BRIDGE_FUNC void NxpAdafruit9Dof_destroy(NxpAdafruit9Dof *imu);

BRIDGE_FUNC void NxpAdafruit9Dof_calibrate(NxpAdafruit9Dof *imu, int samples);

BRIDGE_FUNC double NxpAdafruit9Dof_getGyroX(NxpAdafruit9Dof *imu);

BRIDGE_FUNC double NxpAdafruit9Dof_getGyroY(NxpAdafruit9Dof *imu);

BRIDGE_FUNC double NxpAdafruit9Dof_getGyroZ(NxpAdafruit9Dof *imu);

BRIDGE_FUNC double NxpAdafruit9Dof_getAccelX(NxpAdafruit9Dof *imu);

BRIDGE_FUNC double NxpAdafruit9Dof_getAccelY(NxpAdafruit9Dof *imu);

BRIDGE_FUNC double NxpAdafruit9Dof_getAccelZ(NxpAdafruit9Dof *imu);

BRIDGE_FUNC void NxpAdafruit9Dof_setGyroX(NxpAdafruit9Dof *imu, double newGyroX);

BRIDGE_FUNC void NxpAdafruit9Dof_setGyroY(NxpAdafruit9Dof *imu, double newGyroY);

BRIDGE_FUNC void NxpAdafruit9Dof_setGyroZ(NxpAdafruit9Dof *imu, double newGyroZ);


////////////////////////////////////////////////////////////////////////////////
/// Mpu6050Imu bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC Mpu6050Imu *Mpu6050Imu_create(bool createDevice, int deviceId);

BRIDGE_FUNC void Mpu6050Imu_destroy(Mpu6050Imu *imu);

BRIDGE_FUNC void Mpu6050Imu_calibrate(Mpu6050Imu *imu, int samples);

BRIDGE_FUNC double Mpu6050Imu_getGyroX(Mpu6050Imu *imu);

BRIDGE_FUNC double Mpu6050Imu_getGyroY(Mpu6050Imu *imu);

BRIDGE_FUNC double Mpu6050Imu_getGyroZ(Mpu6050Imu *imu);

BRIDGE_FUNC double Mpu6050Imu_getAccelX(Mpu6050Imu *imu);

BRIDGE_FUNC double Mpu6050Imu_getAccelY(Mpu6050Imu *imu);

BRIDGE_FUNC double Mpu6050Imu_getAccelZ(Mpu6050Imu *imu);

BRIDGE_FUNC void Mpu6050Imu_setGyroX(Mpu6050Imu *imu, double newGyroX);

BRIDGE_FUNC void Mpu6050Imu_setGyroY(Mpu6050Imu *imu, double newGyroY);

BRIDGE_FUNC void Mpu6050Imu_setGyroZ(Mpu6050Imu *imu, double newGyroZ);


////////////////////////////////////////////////////////////////////////////////
/// INA260PowerSensor bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC INA260PowerSensor *INA260PowerSensor_create();

BRIDGE_FUNC void INA260PowerSensor_destroy(INA260PowerSensor *vmon);

BRIDGE_FUNC double INA260PowerSensor_getCurrent(INA260PowerSensor *vmon);

BRIDGE_FUNC double INA260PowerSensor_getVoltage(INA260PowerSensor *vmon);

BRIDGE_FUNC double INA260PowerSensor_getPower(INA260PowerSensor *vmon);


////////////////////////////////////////////////////////////////////////////////
/// StatusLED bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC StatusLED *StatusLED_create(unsigned int pin);

BRIDGE_FUNC void StatusLED_destroy(StatusLED *led);


////////////////////////////////////////////////////////////////////////////////
/// GPIOPin bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC GPIOPin *GPIOPin_create(unsigned int pin);

BRIDGE_FUNC void GPIOPin_destroy(GPIOPin *dev);

BRIDGE_FUNC void GPIOPin_setMode(GPIOPin *dev, int mode);

BRIDGE_FUNC void GPIOPin_setLevel(GPIOPin *dev, int level);

BRIDGE_FUNC int GPIOPin_getLevel(GPIOPin *dev);

BRIDGE_FUNC void GPIOPin_setPwmValue(GPIOPin *dev, uint8_t val);

BRIDGE_FUNC uint8_t GPIOPin_getPwmValue(GPIOPin *dev);

BRIDGE_FUNC void GPIOPin_setPwmFrequency(GPIOPin *dev, unsigned int freq);

BRIDGE_FUNC unsigned int GPIOPin_getPwmFrequency(GPIOPin *dev);


////////////////////////////////////////////////////////////////////////////////
/// AudioManager bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC size_t AudioManager_getPlaybackDevicesCount();

BRIDGE_FUNC void AudioManager_getPlaybackDevice(size_t index, uint32_t *id, char **name, bool *isDefault, uint8_t *type);

BRIDGE_FUNC int AudioManager_playSound(const char *filename);

BRIDGE_FUNC int AudioManager_playSoundWithDevice(const char *filename, uint32_t playbackDeviceId, const char *playbackDeviceName, bool playbackDeviceIsDefault, uint8_t playbackDeviceType);

BRIDGE_FUNC void AudioManager_stopJob(int jobId);


////////////////////////////////////////////////////////////////////////////////
/// PID Bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC PID *PID_create(double kp, double ki, double kd, double kf, double min, double max);

BRIDGE_FUNC void PID_destroy(PID *pid);

BRIDGE_FUNC double PID_getKp(PID *pid);

BRIDGE_FUNC void PID_setKp(PID *pid, double kp);

BRIDGE_FUNC double PID_getKi(PID *pid);

BRIDGE_FUNC void PID_setKi(PID *pid, double ki);

BRIDGE_FUNC double PID_getKd(PID *pid);

BRIDGE_FUNC void PID_setKd(PID *pid, double kd);

BRIDGE_FUNC double PID_getKf(PID *pid);

BRIDGE_FUNC void PID_setKf(PID *pid, double kf);

BRIDGE_FUNC double PID_getMin(PID *pid);

BRIDGE_FUNC void PID_setMin(PID *pid, double min);

BRIDGE_FUNC double PID_getMax(PID *pid);

BRIDGE_FUNC void PID_setMax(PID *pid, double max);

BRIDGE_FUNC double PID_getSetpoint(PID *pid);

BRIDGE_FUNC void PID_setSetpoint(PID *pid, double setpoint);

BRIDGE_FUNC void PID_reset(PID *pid);

BRIDGE_FUNC double PID_getOutput(PID *pid, double currentPv);
