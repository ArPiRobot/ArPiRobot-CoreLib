#include <arpirobot/bridge.hpp>
#include <arpirobot/core/network.hpp>
#include <arpirobot/core/log.hpp>

////////////////////////////////////////////////////////////////////////////////
/// General/Helper
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC char *returnableString(std::string str){
    char *buffer = new char[str.length() + 1];
    strcpy(buffer, str.c_str());
    return buffer;
}

BRIDGE_FUNC void freeString(char *str){
    delete[] str;
}

////////////////////////////////////////////////////////////////////////////////
/// BaseRobot Bridge (arpirobot/core/robot.hpp)
////////////////////////////////////////////////////////////////////////////////

BridgeBaseRobot::BridgeBaseRobot(void (*robotStartedPtr)(void), 
                        void (*robotEnabledPtr)(void), 
                        void (*robotDisabledPtr)(void), 
                        void (*enabledPeriodicPtr)(void), 
                        void (*disabledPeriodicPtr)(void), 
                        void (*periodicPtr)(void),
                        RobotProfile profile) : BaseRobot(profile),
                        robotStartedPtr(robotStartedPtr),
                        robotEnabledPtr(robotEnabledPtr),
                        robotDisabledPtr(robotDisabledPtr),
                        enabledPeriodicPtr(enabledPeriodicPtr),
                        disabledPeriodicPtr(disabledPeriodicPtr),
                        periodicPtr(periodicPtr){
    
}

void BridgeBaseRobot::robotStarted(){
    robotStartedPtr();
}

void BridgeBaseRobot::robotEnabled(){
    robotEnabledPtr();
}

void BridgeBaseRobot::robotDisabled(){
    robotDisabledPtr();
}

void BridgeBaseRobot::enabledPeriodic(){
    enabledPeriodicPtr();
}

void BridgeBaseRobot::disabledPeriodic(){
    disabledPeriodicPtr();
}

void BridgeBaseRobot::periodic(){
    periodicPtr();
}

BRIDGE_FUNC BaseRobot* BaseRobot_create(void (*robotStarted)(void), 
                        void (*robotEnabled)(void), 
                        void (*robotDisabled)(void), 
                        void (*enabledPeriodic)(void), 
                        void (*disabledPeriodic)(void), 
                        void (*periodic)(void),
                        int mainSchedulerThreads,
                        int periodicFunctionRate,
                        int maxGamepadDataAge){
    RobotProfile profile;

    profile.mainSchedulerThreads = mainSchedulerThreads;
    profile.periodicFunctionRate = periodicFunctionRate;
    profile.maxGamepadDataAge = maxGamepadDataAge;

    BaseRobot *robot = new BridgeBaseRobot(robotStarted, robotEnabled, robotDisabled, 
        enabledPeriodic, disabledPeriodic, periodic, profile);
    return robot;
}

BRIDGE_FUNC void BaseRobot_destroy(BaseRobot *robot){
    delete robot;
}

BRIDGE_FUNC void BaseRobot_start(BaseRobot *robot){
    robot->start();
}

BRIDGE_FUNC void BaseRobot_feedWatchdog(BaseRobot *robot){
    robot->feedWatchdog();
}

////////////////////////////////////////////////////////////////////////////////
/// NetworkTable Bridge (arpirobot/core/network.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC void NetworkTable_set(const char *key, const char *value){
    NetworkTable::set(std::string(key), std::string(value));
}

BRIDGE_FUNC char *NetworkTable_get(const char *key){
    return returnableString(NetworkTable::get(key));
}

BRIDGE_FUNC bool NetworkTable_has(const char *key){
    return NetworkTable::has(key);
}

////////////////////////////////////////////////////////////////////////////////
/// Logger Bridge (arpirobot/core/log.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC void Logger_logDebug(const char *message){
    Logger::logDebug(std::string(message));
}

BRIDGE_FUNC void Logger_logInfo(const char *message){
    Logger::logInfo(std::string(message));
}

BRIDGE_FUNC void Logger_logWarning(const char *message){
    Logger::logWarning(std::string(message));
}

BRIDGE_FUNC void Logger_logError(const char *message){
    Logger::logError(std::string(message));
}

BRIDGE_FUNC void Logger_logDebugFrom(const char *source, const char *message){
    Logger::logDebugFrom(std::string(source), std::string(message));
}

BRIDGE_FUNC void Logger_logInfoFrom(const char *source, const char *message){
    Logger::logInfoFrom(std::string(source), std::string(message));
}

BRIDGE_FUNC void Logger_logWarningFrom(const char *source, const char *message){
    Logger::logWarningFrom(std::string(source), std::string(message));
}

BRIDGE_FUNC void Logger_logErrorFrom(const char *source, const char *message){
    Logger::logErrorFrom(std::string(source), std::string(message));
}

BRIDGE_FUNC void Logger_logNewline(){
    Logger::logNewline();
}


////////////////////////////////////////////////////////////////////////////////
/// BaseDevice bridge (arpirobot/core/device.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC char *BaseDevice_getDeviceName(BaseDevice *device){
    return returnableString(device->getDeviceName());
}


////////////////////////////////////////////////////////////////////////////////
/// Gamepad bridge (arpirobot/devices/gamepad.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC Gamepad *Gamepad_create(int controllerNum){
    Gamepad *gamepad = new Gamepad(controllerNum);
    return gamepad;
}

BRIDGE_FUNC void Gamepad_destroy(Gamepad *gamepad){
    delete gamepad;
}

BRIDGE_FUNC int Gamepad_getControllerNum(Gamepad *gamepad){
    return gamepad->getControllerNum();
}

BRIDGE_FUNC double Gamepad_getAxis(Gamepad *gamepad, int axisNum, double deadband){
    return gamepad->getAxis(axisNum, deadband);
}

BRIDGE_FUNC bool Gamepad_getButton(Gamepad *gamepad, int buttonNum){
    return gamepad->getButton(buttonNum);
}

BRIDGE_FUNC int Gamepad_getDpad(Gamepad *gamepad, int dpadNum){
    return gamepad->getDpad(dpadNum);
}


////////////////////////////////////////////////////////////////////////////////
/// MotorController bridge (arpirobot/core/device.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC bool MotorController_isInverted(MotorController *motor){
    return motor->isInverted();
}

BRIDGE_FUNC void MotorController_setInverted(MotorController *motor, bool inverted){
    motor->setInverted(inverted);
}

BRIDGE_FUNC bool MotorController_isBrakeMode(MotorController *motor){
    return motor->isBrakeMode();
}

BRIDGE_FUNC void MotorController_setBrakeMode(MotorController *motor, bool brakeMode){
    motor->setBrakeMode(brakeMode);
}

BRIDGE_FUNC void MotorController_setSpeed(MotorController *motor, double speed){
    motor->setSpeed(speed);
}

BRIDGE_FUNC double MotorController_getSpeed(MotorController *motor){
    return motor->getSpeed();
}


////////////////////////////////////////////////////////////////////////////////
/// AdafruitMotorHatMotor bridge (arpirobot/devices/adafruitmotorhat.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC AdafruitMotorHatMotor *AdafruitMotorHatMotor_create(int motorNum, int address, bool remapNumbers){
    return new AdafruitMotorHatMotor(motorNum, address, remapNumbers);
}

BRIDGE_FUNC void AdafruitMotorHatMotor_destroy(AdafruitMotorHatMotor *motor){
    delete motor;
}


////////////////////////////////////////////////////////////////////////////////
/// ArcadeDriveHelper bridge (arpirobot/core/drive.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC ArcadeDriveHelper *ArcadeDriveHelper_create(MotorController **leftMotors, size_t leftMotorCount, 
        MotorController **rightMotors, size_t rightMotorCount){
    std::vector<MotorController*> lm;
    std::vector<MotorController*> rm;
    lm.reserve(leftMotorCount);
    rm.reserve(rightMotorCount);
    for(int i = 0; i < leftMotorCount; ++i){
        lm.push_back(leftMotors[i]);
    }
    for(int i = 0; i < rightMotorCount; ++i){
        rm.push_back(rightMotors[i]);
    }
    return new ArcadeDriveHelper(lm, rm);
}

BRIDGE_FUNC void ArcadeDriveHelper_destroy(ArcadeDriveHelper *helper){
    delete helper;
}

BRIDGE_FUNC void ArcadeDriveHelper_updateSpeed(ArcadeDriveHelper *helper, double speed){
    helper->updateSpeed(speed);
}

BRIDGE_FUNC void ArcadeDriveHelper_updateRotation(ArcadeDriveHelper *helper, double rotation){
    helper->updateRotation(rotation);
}

BRIDGE_FUNC void ArcadeDriveHelper_update(ArcadeDriveHelper *helper, double speed, double rotation){
    helper->update(speed, rotation);
}
