#include <arpirobot/bridge.hpp>
#include <arpirobot/core/network.hpp>
#include <arpirobot/core/log.hpp>
#include <arpirobot/core/drive.hpp>


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
                        int maxGamepadDataAge,
                        int actionFunctionPeriod){
    RobotProfile profile;

    profile.mainSchedulerThreads = mainSchedulerThreads;
    profile.periodicFunctionRate = periodicFunctionRate;
    profile.maxGamepadDataAge = maxGamepadDataAge;
    profile.actionFunctionPeriod = actionFunctionPeriod;

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

BRIDGE_FUNC void Gamepad_setAxisTransform(Gamepad *gamepad, int axisNum, BaseAxisTransform *transform){
    gamepad->setAxisTransform(axisNum, transform);
}

BRIDGE_FUNC void Gamepad_clearAxisTransform(Gamepad *gamepad, int axisNum){
    gamepad->clearAxisTransform(axisNum);
}


////////////////////////////////////////////////////////////////////////////////
/// ButtonPressedTrigger bridge (arpirobot/devices/gamepad.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC ButtonPressedTrigger *ButtonPressedTrigger_create(Gamepad *gamepad, int buttonNum, 
    Action *targetAction, bool doRestart){
    return new ButtonPressedTrigger(gamepad, buttonNum, targetAction, doRestart);
}

BRIDGE_FUNC void ButtonPressedTrigger_destroy(ButtonPressedTrigger *trigger){
    delete trigger;
}


////////////////////////////////////////////////////////////////////////////////
/// ButtonReleasedTrigger bridge (arpirobot/devices/gamepad.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC ButtonReleasedTrigger *ButtonReleasedTrigger_create(Gamepad *gamepad, int buttonNum, 
        Action *targetAction, bool doRestart){
    return new ButtonReleasedTrigger(gamepad, buttonNum, targetAction, doRestart);
}

BRIDGE_FUNC void ButtonReleasedTrigger_destroy(ButtonReleasedTrigger *trigger){
    delete trigger;
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
/// L298Motor bridge (arpirobot/devices/l298n.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC L298NMotor *L298NMotor_create(int in1Pin, int in2Pin, int pwmPin){
    return new L298NMotor(in1Pin, in2Pin, pwmPin);
}

BRIDGE_FUNC void L298NMotor_destroy(L298NMotor *motor){
    delete motor;
}


////////////////////////////////////////////////////////////////////////////////
/// TB6612Motor bridge (arpirobot/devices/tb6612.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC TB6612Motor *TB6612Motor_create(int in1Pin, int in2Pin, int pwmPin){
    return new TB6612Motor(in1Pin, in2Pin, pwmPin);
}

BRIDGE_FUNC void TB6612Motor_destroy(TB6612Motor *motor){
    delete motor;
}


////////////////////////////////////////////////////////////////////////////////
/// DRV8833Motor bridge (arpirobot/devices/drv8833.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC DRV8833Motor *DRV8833Motor_create(int in1Pin, int in2Pin, int slpPin){
    return new DRV8833Motor(in1Pin, in2Pin, slpPin);
}

BRIDGE_FUNC void DRV8833Motor_destroy(DRV8833Motor *motor){
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


////////////////////////////////////////////////////////////////////////////////
/// TankDriveHelper bridge (arpirobot/core/drive.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC TankDriveHelper *TankDriveHelper_create(MotorController **leftMotors, size_t leftMotorCount, 
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
    return new TankDriveHelper(lm, rm);
}

BRIDGE_FUNC void TankDriveHelper_destroy(TankDriveHelper *helper){
    delete helper;
}

BRIDGE_FUNC void TankDriveHelper_updateLeftSpeed(TankDriveHelper *helper, double newLeftSpeed){
    helper->updateLeftSpeed(newLeftSpeed);
}

BRIDGE_FUNC void TankDriveHelper_updateRightSpeed(TankDriveHelper *helper, double newRightSpeed){
    helper->updateRightSpeed(newRightSpeed);
}

BRIDGE_FUNC void TankDriveHelper_update(TankDriveHelper *helper, double newLeftSpeed, double newRightSpeed){
    helper->update(newLeftSpeed, newRightSpeed);
}


////////////////////////////////////////////////////////////////////////////////
/// BaseAxisTransform bridge (arpirobot/core/drive.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC double BaseAxisTransform_applyTransform(BaseAxisTransform *transform, double rawAxisValue){
    return transform->applyTransform(rawAxisValue);
}


////////////////////////////////////////////////////////////////////////////////
/// SquareRootAxisTransform bridge (arpirobot/core/drive.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC SquareRootAxisTransform *SquareRootAxisTransform_create(){
    return new SquareRootAxisTransform();
}

BRIDGE_FUNC void SquareRootAxisTransform_destroy(SquareRootAxisTransform *transform){
    delete transform;
}


////////////////////////////////////////////////////////////////////////////////
/// CubicAxisTransform bridge (arpirobot/core/drive.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC CubicAxisTransform *CubicAxisTransform_create(double minPower, double midPower){
    return new CubicAxisTransform(minPower, midPower);
}

BRIDGE_FUNC void CubicAxisTransform_destroy(CubicAxisTransform *transform){
    delete transform;
}


////////////////////////////////////////////////////////////////////////////////
/// Action Bridge (arpirobot/core/action.hpp)
////////////////////////////////////////////////////////////////////////////////

BridgeAction::BridgeAction(void (*beginPtr)(void),
        void (*processPtr)(void),
        void (*finishPtr)(bool),
        bool (*shouldContinuePtr)(void)) : beginPtr(beginPtr), processPtr(processPtr), 
        finishPtr(finishPtr), shouldContinuePtr(shouldContinuePtr){
    
}

void BridgeAction::begin(){
    beginPtr();
}

void BridgeAction::process(){
    processPtr();
}

void BridgeAction::finish(bool interrupted){
    finishPtr(interrupted);
}

bool BridgeAction::shouldContinue(){
    return shouldContinuePtr();
}

BRIDGE_FUNC Action *Action_create(void (*beginPtr)(void),
        void (*processPtr)(void),
        void (*finishPtr)(bool),
        bool (*shouldContinuePtr)(void)){
    return new BridgeAction(beginPtr, processPtr, finishPtr, shouldContinuePtr);
}

BRIDGE_FUNC void Action_destroy(Action *action){
    delete action;
}

BRIDGE_FUNC void Action_lockDevices(Action *action, BaseDevice **devices, size_t deviceCount){
    std::vector<BaseDevice*> devs;
    devs.reserve(deviceCount);
    for(int i = 0; i < deviceCount; ++i){
        devs.push_back(devices[i]);
    }
    action->lockDevices(devs);
}

BRIDGE_FUNC void Action_lockDevice(Action *action, BaseDevice *device){
    action->lockDevice(device);
}

BRIDGE_FUNC bool Action_isRunning(Action *action){
    return action->isRunning();
}


////////////////////////////////////////////////////////////////////////////////
/// ActionManager Bridge (arpirobot/core/action.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC bool ActionManager_startAction(Action *action){
    return ActionManager::startAction(action);
}

BRIDGE_FUNC bool ActionManager_stopAction(Action *action){
    return ActionManager::stopAction(action);
}

BRIDGE_FUNC void ActionManager_addTrigger(BaseActionTrigger *trigger){
    ActionManager::addTrigger(trigger);
}

BRIDGE_FUNC void ActionManager_removeTrigger(BaseActionTrigger *trigger){
    ActionManager::removeTrigger(trigger);
}


////////////////////////////////////////////////////////////////////////////////
/// ActionSeries Bridge (arpirobot/core/action.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC ActionSeries *ActionSeries_create(Action **actions, size_t actionCount, Action* finishAction){
    std::vector<Action*> actionsVector;
    actionsVector.reserve(actionCount);
    for(int i = 0; i < actionCount; ++i){
        actionsVector.push_back(actions[i]);
    }
    return new ActionSeries(actionsVector, finishAction);
}

BRIDGE_FUNC void ActionSeries_destroy(ActionSeries *actionSeries){
    delete actionSeries;
}


////////////////////////////////////////////////////////////////////////////////
/// BaseArduinoInterface
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC void BaseArduinoInterface_begin(BaseArduinoInterface *arduino){
    arduino->begin();
}

BRIDGE_FUNC void BaseArduinoInterface_addDevice(BaseArduinoInterface *arduino, ArduinoDevice *device){
    arduino->addDevice(device);
}

BRIDGE_FUNC bool BaseArduinoInterface_isReady(BaseArduinoInterface *arduino){
    return arduino->isReady();
}


////////////////////////////////////////////////////////////////////////////////
/// ArduinoUartInterface
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC ArduinoUartInterface *ArduinoUartInterface_create(const char *tty, int baud){
    return new ArduinoUartInterface(std::string(tty), baud);
}

BRIDGE_FUNC void ArduinoUartInterface_destroy(ArduinoUartInterface *interface){
    delete interface;
}


////////////////////////////////////////////////////////////////////////////////
/// ArduinoDevice
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC char *ArduinoDevice_getDeviceName(ArduinoDevice *device){
    return returnableString(device->getDeviceName());
}


////////////////////////////////////////////////////////////////////////////////
/// VoltageMonitor
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC VoltageMonitor *VoltageMonitor_create(const char *pin, double vboard, int r1, 
        int r2, bool createDevice, int deviceId){
    return new VoltageMonitor(pin, vboard, r1, r2, createDevice, deviceId);
}

BRIDGE_FUNC void VoltageMonitor_destroy(VoltageMonitor *vmon){
    delete vmon;
}

BRIDGE_FUNC double VoltageMonitor_getVoltage(VoltageMonitor *vmon){
    return vmon->getVoltage();
}

BRIDGE_FUNC void VoltageMonitor_makeMainVmon(VoltageMonitor *vmon){
    vmon->makeMainVmon();
}


////////////////////////////////////////////////////////////////////////////////
/// Ultrasonic4Pin (arpirobot/arduino/sensor.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC Ultrasonic4Pin *Ultrasonic4Pin_create(const char *triggerPin, const char *echoPin, 
        bool createDevice, int deviceId){
    return new Ultrasonic4Pin(std::string(triggerPin), std::string(echoPin), createDevice, deviceId);
}

BRIDGE_FUNC void Ultrasonic4Pin_destroy(Ultrasonic4Pin *usonic){
    delete usonic;
}

BRIDGE_FUNC int Ultrasonic4Pin_getDistance(Ultrasonic4Pin *usonic){
    return usonic->getDistance();
}


////////////////////////////////////////////////////////////////////////////////
/// SingleEncode bridge (arpirobot/arduino/sensor.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC SingleEncoder *SingleEncoder_create(const char *pin, bool useInternalPullup, 
        bool createDevice, int deviceId){
    return new SingleEncoder(std::string(pin), useInternalPullup, createDevice, deviceId);
}

BRIDGE_FUNC void SingleEncoder_destroy(SingleEncoder *enc){
    delete enc;
}

BRIDGE_FUNC int SingleEncoder_getPosition(SingleEncoder *enc){
    return enc->getPosition();
}

BRIDGE_FUNC void SingleEncoder_setPosition(SingleEncoder *enc, int newPosition){
    enc->setPosition(newPosition);
}


////////////////////////////////////////////////////////////////////////////////
/// IRReflectorModule bridge (arpirobot/arduino/sensor.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC IRReflectorModule *IRReflectorModule_create(const char *digitalPin, const char *analogPin, 
        bool createDevice, int deviceId){
    return new IRReflectorModule(std::string(digitalPin), std::string(analogPin));
}

BRIDGE_FUNC void IRReflectorModule_destroy(IRReflectorModule *ir){
    delete ir;
}

BRIDGE_FUNC bool IRReflectorModule_getDigitalValue(IRReflectorModule *ir){
    return ir->getDigitalValue();
}

BRIDGE_FUNC int IRReflectorModule_getAnalogValue(IRReflectorModule *ir){
    return ir->getAnalogValue();
}


////////////////////////////////////////////////////////////////////////////////
/// OldAdafruit9Dof bridge (arpirobot/arduino/sensor.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC OldAdafruit9Dof *OldAdafruit9Dof_create(bool createDevice, int deviceId){
    return new OldAdafruit9Dof();
}

BRIDGE_FUNC void OldAdafruit9Dof_destroy(OldAdafruit9Dof *imu){
    delete imu;
}

BRIDGE_FUNC double OldAdafruit9Dof_getGyroX(OldAdafruit9Dof *imu){
    return imu->getGyroX();
}

BRIDGE_FUNC double OldAdafruit9Dof_getGyroY(OldAdafruit9Dof *imu){
    return imu->getGyroY();
}

BRIDGE_FUNC double OldAdafruit9Dof_getGyroZ(OldAdafruit9Dof *imu){
    return imu->getGyroZ();
}

BRIDGE_FUNC double OldAdafruit9Dof_getAccelX(OldAdafruit9Dof *imu){
    return imu->getAccelX();
}

BRIDGE_FUNC double OldAdafruit9Dof_getAccelY(OldAdafruit9Dof *imu){
    return imu->getAccelY();
}

BRIDGE_FUNC double OldAdafruit9Dof_getAccelZ(OldAdafruit9Dof *imu){
    return imu->getAccelZ();
}

BRIDGE_FUNC void OldAdafruit9Dof_setGyroX(OldAdafruit9Dof *imu, double newGyroX){
    imu->setGyroX(newGyroX);
}

BRIDGE_FUNC void OldAdafruit9Dof_setGyroY(OldAdafruit9Dof *imu, double newGyroY){
    imu->setGyroY(newGyroY);
}

BRIDGE_FUNC void OldAdafruit9Dof_setGyroZ(OldAdafruit9Dof *imu, double newGyroZ){
    imu->setGyroZ(newGyroZ);
}


////////////////////////////////////////////////////////////////////////////////
/// NxpAdafruit9Dof bridge (arpirobot/arduino/sensor.hpp)
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC NxpAdafruit9Dof *NxpAdafruit9Dof_create(bool createDevice, int deviceId){
    return new NxpAdafruit9Dof();
}

BRIDGE_FUNC void NxpAdafruit9Dof_destroy(NxpAdafruit9Dof *imu){
    delete imu;
}

BRIDGE_FUNC double NxpAdafruit9Dof_getGyroX(NxpAdafruit9Dof *imu){
    return imu->getGyroX();
}

BRIDGE_FUNC double NxpAdafruit9Dof_getGyroY(NxpAdafruit9Dof *imu){
    return imu->getGyroY();
}

BRIDGE_FUNC double NxpAdafruit9Dof_getGyroZ(NxpAdafruit9Dof *imu){
    return imu->getGyroZ();
}

BRIDGE_FUNC double NxpAdafruit9Dof_getAccelX(NxpAdafruit9Dof *imu){
    return imu->getAccelX();
}

BRIDGE_FUNC double NxpAdafruit9Dof_getAccelY(NxpAdafruit9Dof *imu){
    return imu->getAccelY();
}

BRIDGE_FUNC double NxpAdafruit9Dof_getAccelZ(NxpAdafruit9Dof *imu){
    return imu->getAccelZ();
}

BRIDGE_FUNC void NxpAdafruit9Dof_setGyroX(NxpAdafruit9Dof *imu, double newGyroX){
    imu->setGyroX(newGyroX);
}

BRIDGE_FUNC void NxpAdafruit9Dof_setGyroY(NxpAdafruit9Dof *imu, double newGyroY){
    imu->setGyroY(newGyroY);
}

BRIDGE_FUNC void NxpAdafruit9Dof_setGyroZ(NxpAdafruit9Dof *imu, double newGyroZ){
    imu->setGyroZ(newGyroZ);
}