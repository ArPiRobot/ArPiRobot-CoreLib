#pragma once

#include <arpirobot/core/robot.hpp>
#include <arpirobot/core/log.hpp>
#include <arpirobot/devices/gamepad.hpp>
#include <arpirobot/core/network.hpp>
#include <arpirobot/devices/adafruitmotorhat.hpp>
#include <arpirobot/core/drive.hpp>
#include <arpirobot/core/device.hpp>
#include <arpirobot/core/action.hpp>
#include <arpirobot/arduino/iface.hpp>
#include <arpirobot/arduino/device.hpp>
#include <arpirobot/arduino/sensor.hpp>
#include <string>

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
                        RobotProfile profile);
    
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
};


BRIDGE_FUNC BaseRobot* BaseRobot_create(void (*robotStarted)(void), 
                        void (*robotEnabled)(void), 
                        void (*robotDisabled)(void), 
                        void (*enabledPeriodic)(void), 
                        void (*disabledPeriodic)(void), 
                        void (*periodic)(void),
                        int mainSchedulerThreads,
                        int periodicFunctionRate,
                        int maxGamepadDataAge,
                        int actionFunctionPeriod);

BRIDGE_FUNC void BaseRobot_destroy(BaseRobot *robot);

BRIDGE_FUNC void BaseRobot_start(BaseRobot *robot);

BRIDGE_FUNC void BaseRobot_feedWatchdog(BaseRobot *robot);


////////////////////////////////////////////////////////////////////////////////
/// NetworkTable Bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC void NetworkTable_set(const char *key, const char *value);

BRIDGE_FUNC char *NetworkTable_get(const char *key);

BRIDGE_FUNC bool NetworkTable_has(const char *key);


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
/// BaseDevice bridge (arpirobot/core/device.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC char *BaseDevice_getDeviceName(BaseDevice *device);


////////////////////////////////////////////////////////////////////////////////
/// Gamepad bridge (arpirobot/devices/gamepad.hpp)
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
/// ButtonPressedTrigger bridge (arpirobot/devices/gamepad.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC ButtonPressedTrigger *ButtonPressedTrigger_create(Gamepad *gamepad, int buttonNum, 
    Action *targetAction, bool doRestart);

BRIDGE_FUNC void ButtonPressedTrigger_destroy(ButtonPressedTrigger *trigger);


////////////////////////////////////////////////////////////////////////////////
/// ButtonReleasedTrigger bridge (arpirobot/devices/gamepad.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC ButtonReleasedTrigger *ButtonReleasedTrigger_create(Gamepad *gamepad, int buttonNum, 
    Action *targetAction, bool doRestart);

BRIDGE_FUNC void ButtonReleasedTrigger_destroy(ButtonReleasedTrigger *trigger);


////////////////////////////////////////////////////////////////////////////////
/// MotorController bridge (arpirobot/core/device.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC bool MotorController_isInverted(MotorController *motor);

BRIDGE_FUNC void MotorController_setInverted(MotorController *motor, bool inverted);

BRIDGE_FUNC bool MotorController_isBrakeMode(MotorController *motor);

BRIDGE_FUNC void MotorController_setBrakeMode(MotorController *motor, bool brakeMode);

BRIDGE_FUNC void MotorController_setSpeed(MotorController *motor, double speed);

BRIDGE_FUNC double MotorController_getSpeed(MotorController *motor);


////////////////////////////////////////////////////////////////////////////////
/// AdafruitMotorHatMotor bridge (arpirobot/devices/adafruitmotorhat.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC AdafruitMotorHatMotor *AdafruitMotorHatMotor_create(int motorNum, int address, bool remapNumbers);

BRIDGE_FUNC void AdafruitMotorHatMotor_destroy(AdafruitMotorHatMotor *motor);


////////////////////////////////////////////////////////////////////////////////
/// ArcadeDriveHelper bridge (arpirobot/core/drive.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC ArcadeDriveHelper *ArcadeDriveHelper_create(MotorController **leftMotors, size_t leftMotorCount, 
    MotorController **rightMotors, size_t rightMotorCount);

BRIDGE_FUNC void ArcadeDriveHelper_destroy(ArcadeDriveHelper *helper);

BRIDGE_FUNC void ArcadeDriveHelper_updateSpeed(ArcadeDriveHelper *helper, double speed);

BRIDGE_FUNC void ArcadeDriveHelper_updateRotation(ArcadeDriveHelper *helper, double rotation);

BRIDGE_FUNC void ArcadeDriveHelper_update(ArcadeDriveHelper *helper, double speed, double rotation);


////////////////////////////////////////////////////////////////////////////////
/// TankDriveHelper bridge (arpirobot/core/drive.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC TankDriveHelper *TankDriveHelper_create(MotorController **leftMotors, size_t leftMotorCount, 
    MotorController **rightMotors, size_t rightMotorCount);

BRIDGE_FUNC void TankDriveHelper_destroy(TankDriveHelper *helper);

BRIDGE_FUNC void TankDriveHelper_updateLeftSpeed(TankDriveHelper *helper, double newLeftSpeed);

BRIDGE_FUNC void TankDriveHelper_updateRightSpeed(TankDriveHelper *helper, double newRightSpeed);

BRIDGE_FUNC void TankDriveHelper_update(TankDriveHelper *helper, double newLeftSpeed, double newRightSpeed);


////////////////////////////////////////////////////////////////////////////////
/// BaseAxisTransform bridge (arpirobot/core/drive.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC double BaseAxisTransform_applyTransform(BaseAxisTransform *transform, double rawAxisValue);


////////////////////////////////////////////////////////////////////////////////
/// SquareRootAxisTransform bridge (arpirobot/core/drive.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC SquareRootAxisTransform *SquareRootAxisTransform_create();

BRIDGE_FUNC void SquareRootAxisTransform_destroy(SquareRootAxisTransform *transform);


////////////////////////////////////////////////////////////////////////////////
/// CubicAxisTransform bridge (arpirobot/core/drive.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC CubicAxisTransform *CubicAxisTransform_create(double minPower, double midPower);

BRIDGE_FUNC void CubicAxisTransform_destroy(CubicAxisTransform *transform);


////////////////////////////////////////////////////////////////////////////////
/// Action Bridge (arpirobot/core/action.hpp)
////////////////////////////////////////////////////////////////////////////////


class BridgeAction : public Action{
public:
    BridgeAction(void (*beginPtr)(void),
        void (*processPtr)(void),
        void (*finishPtr)(bool),
        bool (*shouldContinuePtr)(void));

    void begin();
    void process();
    void finish(bool interrupted);
    bool shouldContinue();

private:
    void (*beginPtr)(void);
    void (*processPtr)(void);
    void (*finishPtr)(bool);
    bool (*shouldContinuePtr)(void);
};

BRIDGE_FUNC Action *Action_create(void (*beginPtr)(void),
    void (*processPtr)(void),
    void (*finishPtr)(bool),
    bool (*shouldContinuePtr)(void));

BRIDGE_FUNC void Action_destroy(Action *action);

BRIDGE_FUNC void Action_lockDevices(Action *action, BaseDevice **devices, size_t deviceCount);

BRIDGE_FUNC void Action_lockDevice(Action *action, BaseDevice *device);

BRIDGE_FUNC bool Action_isRunning(Action *action);


////////////////////////////////////////////////////////////////////////////////
/// ActionManager Bridge (arpirobot/core/action.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC bool ActionManager_startAction(Action *action);

BRIDGE_FUNC bool ActionManager_stopAction(Action *action);

BRIDGE_FUNC void ActionManager_addTrigger(BaseActionTrigger *trigger);

BRIDGE_FUNC void ActionManager_removeTrigger(BaseActionTrigger *trigger);


////////////////////////////////////////////////////////////////////////////////
/// ActionSeries Bridge (arpirobot/core/action.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC ActionSeries *ActionSeries_create(Action **actions, size_t actionCount, Action* finishAction);

BRIDGE_FUNC void ActionSeries_destroy(ActionSeries *actionSeries);


////////////////////////////////////////////////////////////////////////////////
/// BaseArduinoInterface
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC void BaseArduinoInterface_begin(BaseArduinoInterface *arduino);

BRIDGE_FUNC void BaseArduinoInterface_addDevice(BaseArduinoInterface *arduino, ArduinoDevice *device);

BRIDGE_FUNC bool BaseArduinoInterface_isReady(BaseArduinoInterface *arduino);


////////////////////////////////////////////////////////////////////////////////
/// ArduinoUartInterface
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC ArduinoUartInterface *ArduinoUartInterface_create(const char *tty, int baud);

BRIDGE_FUNC void ArduinoUartInterface_destroy(ArduinoUartInterface *interface);


////////////////////////////////////////////////////////////////////////////////
/// ArduinoDevice
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC char *ArduinoDevice_getDeviceName(ArduinoDevice *device);


////////////////////////////////////////////////////////////////////////////////
/// VoltageMonitor
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC VoltageMonitor *VoltageMonitor_create(const char *pin, double vboard, int r1, 
    int r2, int deviceId, bool createDevice);

BRIDGE_FUNC void VoltageMonitor_destroy(VoltageMonitor *vmon);

BRIDGE_FUNC double VoltageMonitor_getVoltage(VoltageMonitor *vmon);

BRIDGE_FUNC void VoltageMonitor_makeMainVmon(VoltageMonitor *vmon);
