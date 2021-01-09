#pragma once

#include <arpirobot/core/robot.hpp>
#include <arpirobot/core/log.hpp>
#include <arpirobot/devices/gamepad.hpp>
#include <arpirobot/devices/adafruitmotorhat.hpp>
#include <arpirobot/core/drive.hpp>
#include <arpirobot/core/device.hpp>
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
                        int periodicFunctionRate);

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
