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

/*
 * Many user-facing functions in the CoreLib have two variants
 * One that accepts a reference to an object. The object's lifetime must be sufficient.
 *     The object is persumably static (but doesn't have to be).
 * The other accepts a shared_ptr to a (presumably) dynamically allocated object.
 * 
 * In the bridge, the reference versions are used as the other language (python, etc)
 *     using the bridge allocates and manages the lifetime of the pointers by calling
 *     the create and destroy functions. The bridge thus calls the reference versions
 *     or creates a shared_ptr with no delete fuction specified to prevent early
 *     deletion of the pointers.
 */

#include <arpirobot/bridge.hpp>
#include <arpirobot/core/network/NetworkTable.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/audio/AudioManager.hpp>
#include <arpirobot/core/audio/AudioDeviceInfo.hpp>
//#include <arpirobot/core/drive.hpp>

#include <cstddef>
#include <iostream>
#include <cstring>
#include <memory>
#include <functional>
#include <algorithm>

#define REMOVE_VAL_FROM_VEC(vec, val) vec.erase(std::remove(vec.begin(), vec.end(), val), vec.end())

////////////////////////////////////////////////////////////////////////////////
/// Globals
////////////////////////////////////////////////////////////////////////////////
// Objects allocated using bridge create functions
// Object is allocated using std::make_shared. 
// shared_ptr is kept here until destory called
// Create functions still return raw pointer (b/c C API)
// When bridge is passed a pointer to another ArPiRobot object, it is known
// that said object was allocated using a std::make_shared. Thus the pointer
// can be wrapped in a smart pointer and passed to the C++ API functions
// Note that wrapping in shared_ptr in the bridge functions is required.
// Cannot pass using reference. C++ API assumes referenced objects to be statically
// allocated, so when it wraps them internally in a shared_ptr, they will have no
// deleter. This could cause memory leaks. To avoid this, wrap in bridge with default deleter.
// The C++ objects will then hold a copy of the smart pointer in addition to here
// When the Destroy functions are called the smart_ptrs are removed from this list
// If another ArPiRobot object holds a copy, they remain in scope. If not they
// are deleted. This "replicates" the behavior of Create using new and Destory
// using delete, except for scenarios when the bridge's calling language (eg python)
// calls destory, but the allocated object may still be used by the C++ API.
// For example:
// In python_bindings an object constructed for an action could be started using
// gp0.set_axis_transform(1, SquareRootAxisTransform())
// This should perform the same as the following C++ code
// gp0.setAxisTransform(std::make_shared<SquareRootAxisTransform>())
// However, the python SquareRootAxisTransform object leaves scope.
// When this happens, the python_bindings call destory. If using delete, this would
// destruct and deallocate the transform, leading to undefined behaviors with getting axis values. 
// Using the shared_ptr model, the Gampad object in C++ holds a copy
// of the shared_ptr even though this list no longer does. Thus the transform remains in allocated 
std::vector<std::shared_ptr<void>> bridge_objs;

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
/// BaseRobot Bridge
////////////////////////////////////////////////////////////////////////////////

BridgeBaseRobot::BridgeBaseRobot(void (*robotStartedPtr)(void), 
                        void (*robotEnabledPtr)(void), 
                        void (*robotDisabledPtr)(void), 
                        void (*enabledPeriodicPtr)(void), 
                        void (*disabledPeriodicPtr)(void), 
                        void (*periodicPtr)(void),
                        void (*destroyedPtr)(void),
                        RobotProfile profile) : BaseRobot(profile),
                        robotStartedPtr(robotStartedPtr),
                        robotEnabledPtr(robotEnabledPtr),
                        robotDisabledPtr(robotDisabledPtr),
                        enabledPeriodicPtr(enabledPeriodicPtr),
                        disabledPeriodicPtr(disabledPeriodicPtr),
                        periodicPtr(periodicPtr),
                        destroyedPtr(destroyedPtr){
    
}

BridgeBaseRobot::~BridgeBaseRobot(){
    // Language bindings using bridge must keep object in scope until C++ BaseRobot
    // object is destroyed. This provides a callback allowing the language bindings
    // know when it is safe to destory their objects.
    // This is only relevant if the language bindings pass pointers to functions
    // that are part of an object they create
    if(destroyedPtr != NULL)
        destroyedPtr();
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
                        void (*destroyed)(void),
                        int mainSchedulerThreads,
                        int periodicFunctionRate,
                        int maxGamepadDataAge,
                        int actionFunctionPeriod){
    RobotProfile profile;

    profile.mainSchedulerThreads = mainSchedulerThreads;
    profile.periodicFunctionRate = periodicFunctionRate;
    profile.maxGamepadDataAge = maxGamepadDataAge;
    profile.actionFunctionPeriod = actionFunctionPeriod;

    auto robot = std::make_shared<BridgeBaseRobot>(robotStarted, robotEnabled, robotDisabled, 
        enabledPeriodic, disabledPeriodic, periodic, destroyed, profile);
    bridge_objs.push_back(robot);
    return robot.get();
}

BRIDGE_FUNC void BaseRobot_destroy(BaseRobot *robot){
    REMOVE_VAL_FROM_VEC(bridge_objs, std::shared_ptr<BaseRobot>(robot));
}

BRIDGE_FUNC void BaseRobot_start(BaseRobot *robot, const char *ioProvider){
    robot->start(std::string(ioProvider));
}

BRIDGE_FUNC void BaseRobot_feedWatchdog(BaseRobot *robot){
    robot->feedWatchdog();
}

////////////////////////////////////////////////////////////////////////////////
/// MainVMon Bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC void MainVMon_makeMainVmon(MainVmon *vmon){
    vmon->makeMainVmon();
}


////////////////////////////////////////////////////////////////////////////////
/// NetworkTable Bridge
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

BRIDGE_FUNC bool NetworkTable_changed(const char *key){
    return NetworkTable::changed(key);
}

////////////////////////////////////////////////////////////////////////////////
/// Logger Bridge
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
/// BaseDevice bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC char *BaseDevice_getDeviceName(BaseDevice *device){
    return returnableString(device->getDeviceName());
}


////////////////////////////////////////////////////////////////////////////////
/// Gamepad bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC Gamepad *Gamepad_create(int controllerNum){
    auto gamepad = std::make_shared<Gamepad>(controllerNum);
    bridge_objs.push_back(gamepad);
    return gamepad.get();
}

BRIDGE_FUNC void Gamepad_destroy(Gamepad *gamepad){
    REMOVE_VAL_FROM_VEC(bridge_objs, std::shared_ptr<Gamepad>(gamepad));
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
    gamepad->setAxisTransform(axisNum, std::shared_ptr<BaseAxisTransform>(transform));
}

BRIDGE_FUNC void Gamepad_clearAxisTransform(Gamepad *gamepad, int axisNum){
    gamepad->clearAxisTransform(axisNum);
}


////////////////////////////////////////////////////////////////////////////////
/// ButtonPressedTrigger bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC ButtonPressedTrigger *ButtonPressedTrigger_create(Gamepad *gamepad, int buttonNum, 
    Action *targetAction, bool doRestart){
    auto trigger = std::make_shared<ButtonPressedTrigger>(
        std::shared_ptr<Gamepad>(gamepad), 
        buttonNum, 
        std::shared_ptr<Action>(targetAction), 
        doRestart);
    bridge_objs.push_back(trigger);
    return trigger.get();
}

BRIDGE_FUNC void ButtonPressedTrigger_destroy(ButtonPressedTrigger *trigger){
    REMOVE_VAL_FROM_VEC(bridge_objs, std::shared_ptr<ButtonPressedTrigger>(trigger));
}


////////////////////////////////////////////////////////////////////////////////
/// ButtonReleasedTrigger bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC ButtonReleasedTrigger *ButtonReleasedTrigger_create(Gamepad *gamepad, int buttonNum, 
        Action *targetAction, bool doRestart){
    auto trigger = std::make_shared<ButtonReleasedTrigger>(
        std::shared_ptr<Gamepad>(gamepad), 
        buttonNum, 
        std::shared_ptr<Action>(targetAction), 
        doRestart);
    bridge_objs.push_back(trigger);
    return trigger.get();
}

BRIDGE_FUNC void ButtonReleasedTrigger_destroy(ButtonReleasedTrigger *trigger){
    REMOVE_VAL_FROM_VEC(bridge_objs, std::shared_ptr<ButtonReleasedTrigger>(trigger));
}


////////////////////////////////////////////////////////////////////////////////
/// MotorController bridge
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
/// AdafruitMotorHatMotor bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC AdafruitMotorHatMotor *AdafruitMotorHatMotor_create(int motorNum, int address, bool remapNumbers){
    auto motor = std::make_shared<AdafruitMotorHatMotor>(motorNum, address, remapNumbers);
    bridge_objs.push_back(motor);
    return motor.get();
}

BRIDGE_FUNC void AdafruitMotorHatMotor_destroy(AdafruitMotorHatMotor *motor){
    REMOVE_VAL_FROM_VEC(bridge_objs, std::shared_ptr<AdafruitMotorHatMotor>(motor));
}


////////////////////////////////////////////////////////////////////////////////
/// L298Motor bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC L298NMotor *L298NMotor_create(int in1Pin, int in2Pin, int pwmPin){
    auto motor = std::make_shared<L298NMotor>(in1Pin, in2Pin, pwmPin);
    bridge_objs.push_back(motor);
    return motor.get();
}

BRIDGE_FUNC void L298NMotor_destroy(L298NMotor *motor){
    REMOVE_VAL_FROM_VEC(bridge_objs, std::shared_ptr<L298NMotor>(motor));
}


////////////////////////////////////////////////////////////////////////////////
/// TB6612Motor bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC TB6612Motor *TB6612Motor_create(int in1Pin, int in2Pin, int pwmPin){
    auto motor = std::make_shared<TB6612Motor>(in1Pin, in2Pin, pwmPin);
    bridge_objs.push_back(motor);
    return motor.get();
}

BRIDGE_FUNC void TB6612Motor_destroy(TB6612Motor *motor){
    REMOVE_VAL_FROM_VEC(bridge_objs, std::shared_ptr<TB6612Motor>(motor));
}


////////////////////////////////////////////////////////////////////////////////
/// DRV8833Motor bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC DRV8833Motor *DRV8833Motor_create(int in1Pin, int in2Pin, int slpPin){
    auto motor = std::make_shared<DRV8833Motor>(in1Pin, in2Pin, slpPin);
    bridge_objs.push_back(motor);
    return motor.get();
}

BRIDGE_FUNC void DRV8833Motor_destroy(DRV8833Motor *motor){
    REMOVE_VAL_FROM_VEC(bridge_objs, std::shared_ptr<DRV8833Motor>(motor));
}


////////////////////////////////////////////////////////////////////////////////
/// ArcadeDriveHelper bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC ArcadeDriveHelper *ArcadeDriveHelper_create(MotorController **leftMotors, size_t leftMotorCount, 
        MotorController **rightMotors, size_t rightMotorCount){
    std::vector<std::shared_ptr<MotorController>> lm;
    std::vector<std::shared_ptr<MotorController>> rm;
    lm.reserve(leftMotorCount);
    rm.reserve(rightMotorCount);
    for(int i = 0; i < leftMotorCount; ++i){
        lm.push_back(std::shared_ptr<MotorController>(leftMotors[i]));
    }
    for(int i = 0; i < rightMotorCount; ++i){
        rm.push_back(std::shared_ptr<MotorController>(rightMotors[i]));
    }
    auto helper = std::make_shared<ArcadeDriveHelper>(lm, rm);
    bridge_objs.push_back(helper);
    return helper.get();
}

BRIDGE_FUNC void ArcadeDriveHelper_destroy(ArcadeDriveHelper *helper){
    REMOVE_VAL_FROM_VEC(bridge_objs, std::shared_ptr<ArcadeDriveHelper>(helper));
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
/// TankDriveHelper bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC TankDriveHelper *TankDriveHelper_create(MotorController **leftMotors, size_t leftMotorCount, 
        MotorController **rightMotors, size_t rightMotorCount){
    std::vector<std::shared_ptr<MotorController>> lm;
    std::vector<std::shared_ptr<MotorController>> rm;
    lm.reserve(leftMotorCount);
    rm.reserve(rightMotorCount);
    for(int i = 0; i < leftMotorCount; ++i){
        lm.push_back(std::shared_ptr<MotorController>(leftMotors[i]));
    }
    for(int i = 0; i < rightMotorCount; ++i){
        rm.push_back(std::shared_ptr<MotorController>(rightMotors[i]));
    }
    auto helper = std::make_shared<TankDriveHelper>(lm, rm);
    bridge_objs.push_back(helper);
    return helper.get();
}

BRIDGE_FUNC void TankDriveHelper_destroy(TankDriveHelper *helper){
    REMOVE_VAL_FROM_VEC(bridge_objs, std::shared_ptr<TankDriveHelper>(helper));
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
/// BaseAxisTransform bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC double BaseAxisTransform_applyTransform(BaseAxisTransform *transform, double rawAxisValue){
    return transform->applyTransform(rawAxisValue);
}


////////////////////////////////////////////////////////////////////////////////
/// SquareRootAxisTransform bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC SquareRootAxisTransform *SquareRootAxisTransform_create(){
    auto transform = std::make_shared<SquareRootAxisTransform>();
    bridge_objs.push_back(transform);
    return transform.get();
}

BRIDGE_FUNC void SquareRootAxisTransform_destroy(SquareRootAxisTransform *transform){
    REMOVE_VAL_FROM_VEC(bridge_objs, std::shared_ptr<SquareRootAxisTransform>(transform));
}


////////////////////////////////////////////////////////////////////////////////
/// CubicAxisTransform bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC CubicAxisTransform *CubicAxisTransform_create(double minPower, double midPower){
    auto transform = std::make_shared<CubicAxisTransform>(minPower, midPower);
    bridge_objs.push_back(transform);
    return transform.get();
}

BRIDGE_FUNC void CubicAxisTransform_destroy(CubicAxisTransform *transform){
    REMOVE_VAL_FROM_VEC(bridge_objs, std::shared_ptr<CubicAxisTransform>(transform));
}


////////////////////////////////////////////////////////////////////////////////
/// Action Bridge
////////////////////////////////////////////////////////////////////////////////

BridgeAction::BridgeAction(void (*beginPtr)(void),
        void (*processPtr)(void),
        void (*finishPtr)(bool),
        bool (*shouldContinuePtr)(void),
        void (*destroyedPtr)(void)) : beginPtr(beginPtr), processPtr(processPtr), 
        finishPtr(finishPtr), shouldContinuePtr(shouldContinuePtr), destroyedPtr(destroyedPtr){
    
}

BridgeAction::~BridgeAction(){
    // Language bindings using bridge must keep object in scope until C++ BaseRobot
    // object is destroyed. This provides a callback allowing the language bindings
    // know when it is safe to destory their objects.
    // This is only relevant if the language bindings pass pointers to functions
    // that are part of an object they create
    if(destroyedPtr != NULL)
        destroyedPtr();
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
    // When this is a pointer to a function from ctypes (python)
    // Optimizations regarding this variable cause errors
    // The variable is treated as 32-bit, but ctypes only sets the lowest byte
    // As such the upper three bytes are "garbage" and unless that garbage happens to be zero,
    // this will always be treated as "true"
    // Disabling optimizations keeps this from happening
    volatile bool res = shouldContinuePtr();
    return res;
}

BRIDGE_FUNC Action *Action_create(void (*beginPtr)(void),
        void (*processPtr)(void),
        void (*finishPtr)(bool),
        bool (*shouldContinuePtr)(void),
        void (*destroyedPtr)(void)){
    auto action = std::make_shared<BridgeAction>(beginPtr, processPtr, finishPtr, shouldContinuePtr, destroyedPtr);
    bridge_objs.push_back(action);
    return action.get();
}

BRIDGE_FUNC void Action_destroy(Action *action){
    REMOVE_VAL_FROM_VEC(bridge_objs, std::shared_ptr<Action>(action));
}

BRIDGE_FUNC void Action_lockDevices(Action *action, BaseDevice **devices, size_t deviceCount){
    std::vector<std::shared_ptr<BaseDevice>> devs;
    devs.reserve(deviceCount);
    for(int i = 0; i < deviceCount; ++i){
        devs.push_back(std::shared_ptr<BaseDevice>(devices[i]));
    }
    action->lockDevices(devs);
}

BRIDGE_FUNC void Action_lockDevice(Action *action, BaseDevice *device){
    action->lockDevice(std::shared_ptr<BaseDevice>(device));
}

BRIDGE_FUNC bool Action_isRunning(Action *action){
    return action->isRunning();
}

BRIDGE_FUNC void Action_setProcessPeriodMs(Action *action, int32_t processPeriodMs){
    action->setProcessPeriodMs(processPeriodMs);
}


////////////////////////////////////////////////////////////////////////////////
/// ActionManager Bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC bool ActionManager_startAction(Action *action){
    return ActionManager::startAction(std::shared_ptr<Action>(action));
}

BRIDGE_FUNC bool ActionManager_stopAction(Action *action){
    return ActionManager::stopAction(std::shared_ptr<Action>(action));
}

BRIDGE_FUNC void ActionManager_addTrigger(BaseActionTrigger *trigger){
    ActionManager::addTrigger(std::shared_ptr<BaseActionTrigger>(trigger));
}

BRIDGE_FUNC void ActionManager_removeTrigger(BaseActionTrigger *trigger){
    ActionManager::removeTrigger(std::shared_ptr<BaseActionTrigger>(trigger));
}


////////////////////////////////////////////////////////////////////////////////
/// ActionSeries Bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC ActionSeries *ActionSeries_create(Action **actions, size_t actionCount, Action* finishAction){
    std::vector<std::shared_ptr<Action>> actionsVector;
    actionsVector.reserve(actionCount);
    for(int i = 0; i < actionCount; ++i){
        actionsVector.push_back(std::shared_ptr<Action>(actions[i]));
    }
    auto series = std::make_shared<ActionSeries>(actionsVector, std::shared_ptr<Action>(finishAction));
    bridge_objs.push_back(series);
    return series.get();
}

BRIDGE_FUNC void ActionSeries_destroy(ActionSeries *actionSeries){
    REMOVE_VAL_FROM_VEC(bridge_objs, std::shared_ptr<ActionSeries>(actionSeries));
}


////////////////////////////////////////////////////////////////////////////////
/// BaseArduinoInterface
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC void BaseArduinoInterface_begin(BaseArduinoInterface *arduino){
    arduino->begin();
}

BRIDGE_FUNC void BaseArduinoInterface_addDevice(BaseArduinoInterface *arduino, ArduinoDevice *device){
    arduino->addDevice(std::shared_ptr<ArduinoDevice>(device));
}

BRIDGE_FUNC bool BaseArduinoInterface_isReady(BaseArduinoInterface *arduino){
    return arduino->isReady();
}


////////////////////////////////////////////////////////////////////////////////
/// ArduinoUartInterface
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC ArduinoUartInterface *ArduinoUartInterface_create(const char *tty, int baud){
    auto interface = std::make_shared<ArduinoUartInterface>(std::string(tty), baud);
    bridge_objs.push_back(interface);
    return interface.get();
}

BRIDGE_FUNC void ArduinoUartInterface_destroy(ArduinoUartInterface *interface){
    REMOVE_VAL_FROM_VEC(bridge_objs, std::shared_ptr<ArduinoUartInterface>(interface));
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
    auto vmon = std::make_shared<VoltageMonitor>(pin, vboard, r1, r2, createDevice, deviceId);
    bridge_objs.push_back(vmon);
    return vmon.get();
}

BRIDGE_FUNC void VoltageMonitor_destroy(VoltageMonitor *vmon){
    REMOVE_VAL_FROM_VEC(bridge_objs, std::shared_ptr<VoltageMonitor>(vmon));
}

BRIDGE_FUNC double VoltageMonitor_getVoltage(VoltageMonitor *vmon){
    return vmon->getVoltage();
}

BRIDGE_FUNC void VoltageMonitor_makeMainVmon(VoltageMonitor *vmon){
    vmon->makeMainVmon();
}


////////////////////////////////////////////////////////////////////////////////
/// Ultrasonic4Pin
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC Ultrasonic4Pin *Ultrasonic4Pin_create(const char *triggerPin, const char *echoPin, 
        bool createDevice, int deviceId){
    auto usonic = std::make_shared<Ultrasonic4Pin>(std::string(triggerPin), std::string(echoPin), createDevice, deviceId);
    bridge_objs.push_back(usonic);
    return usonic.get();
}

BRIDGE_FUNC void Ultrasonic4Pin_destroy(Ultrasonic4Pin *usonic){
    REMOVE_VAL_FROM_VEC(bridge_objs, std::shared_ptr<Ultrasonic4Pin>(usonic));
}

BRIDGE_FUNC int Ultrasonic4Pin_getDistance(Ultrasonic4Pin *usonic){
    return usonic->getDistance();
}


////////////////////////////////////////////////////////////////////////////////
/// SingleEncode bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC SingleEncoder *SingleEncoder_create(const char *pin, bool useInternalPullup, 
        bool createDevice, int deviceId){
    auto enc = std::make_shared<SingleEncoder>(std::string(pin), useInternalPullup, createDevice, deviceId);
    bridge_objs.push_back(enc);
    return enc.get();
}

BRIDGE_FUNC void SingleEncoder_destroy(SingleEncoder *enc){
    REMOVE_VAL_FROM_VEC(bridge_objs, std::shared_ptr<SingleEncoder>(enc));
}

BRIDGE_FUNC int SingleEncoder_getPosition(SingleEncoder *enc){
    return enc->getPosition();
}

BRIDGE_FUNC void SingleEncoder_setPosition(SingleEncoder *enc, int newPosition){
    enc->setPosition(newPosition);
}


////////////////////////////////////////////////////////////////////////////////
/// IRReflectorModule bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC IRReflectorModule *IRReflectorModule_create(const char *digitalPin, const char *analogPin, 
        bool createDevice, int deviceId){
    auto ir = std::make_shared<IRReflectorModule>(std::string(digitalPin), std::string(analogPin));
    bridge_objs.push_back(ir);
    return ir.get();
}

BRIDGE_FUNC void IRReflectorModule_destroy(IRReflectorModule *ir){
    REMOVE_VAL_FROM_VEC(bridge_objs, std::shared_ptr<IRReflectorModule>(ir));
}

BRIDGE_FUNC bool IRReflectorModule_getDigitalValue(IRReflectorModule *ir){
    return ir->getDigitalValue();
}

BRIDGE_FUNC int IRReflectorModule_getAnalogValue(IRReflectorModule *ir){
    return ir->getAnalogValue();
}


////////////////////////////////////////////////////////////////////////////////
/// OldAdafruit9Dof bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC OldAdafruit9Dof *OldAdafruit9Dof_create(bool createDevice, int deviceId){
    auto imu = std::make_shared<OldAdafruit9Dof>();
    bridge_objs.push_back(imu);
    return imu.get();
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
/// NxpAdafruit9Dof bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC NxpAdafruit9Dof *NxpAdafruit9Dof_create(bool createDevice, int deviceId){
    auto imu = std::make_shared<NxpAdafruit9Dof>();
    bridge_objs.push_back(imu);
    return imu.get();
}

BRIDGE_FUNC void NxpAdafruit9Dof_destroy(NxpAdafruit9Dof *imu){
    REMOVE_VAL_FROM_VEC(bridge_objs, std::shared_ptr<NxpAdafruit9Dof>(imu));
}

BRIDGE_FUNC void NxpAdafruit9Dof_calibrate(NxpAdafruit9Dof *imu, int samples){
    imu->calibrate(samples);
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


////////////////////////////////////////////////////////////////////////////////
/// Mpu6050Imu bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC Mpu6050Imu *Mpu6050Imu_create(bool createDevice, int deviceId){
    auto imu = std::make_shared<Mpu6050Imu>();
    bridge_objs.push_back(imu);
    return imu.get();
}

BRIDGE_FUNC void Mpu6050Imu_destroy(Mpu6050Imu *imu){
    REMOVE_VAL_FROM_VEC(bridge_objs, std::shared_ptr<Mpu6050Imu>(imu));
}

BRIDGE_FUNC void Mpu6050Imu_calibrate(Mpu6050Imu *imu, int samples){
    imu->calibrate(samples);
}

BRIDGE_FUNC double Mpu6050Imu_getGyroX(Mpu6050Imu *imu){
    return imu->getGyroX();
}

BRIDGE_FUNC double Mpu6050Imu_getGyroY(Mpu6050Imu *imu){
    return imu->getGyroY();
}

BRIDGE_FUNC double Mpu6050Imu_getGyroZ(Mpu6050Imu *imu){
    return imu->getGyroZ();
}

BRIDGE_FUNC double Mpu6050Imu_getAccelX(Mpu6050Imu *imu){
    return imu->getAccelX();
}

BRIDGE_FUNC double Mpu6050Imu_getAccelY(Mpu6050Imu *imu){
    return imu->getAccelY();
}

BRIDGE_FUNC double Mpu6050Imu_getAccelZ(Mpu6050Imu *imu){
    return imu->getAccelZ();
}

BRIDGE_FUNC void Mpu6050Imu_setGyroX(Mpu6050Imu *imu, double newGyroX){
    imu->setGyroX(newGyroX);
}

BRIDGE_FUNC void Mpu6050Imu_setGyroY(Mpu6050Imu *imu, double newGyroY){
    imu->setGyroY(newGyroY);
}

BRIDGE_FUNC void Mpu6050Imu_setGyroZ(Mpu6050Imu *imu, double newGyroZ){
    imu->setGyroZ(newGyroZ);
}


////////////////////////////////////////////////////////////////////////////////
/// INA260PowerSensor bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC INA260PowerSensor *INA260PowerSensor_create(){
    auto vmon = std::make_shared<INA260PowerSensor>();
    bridge_objs.push_back(vmon);
    return vmon.get();
}

BRIDGE_FUNC void INA260PowerSensor_destroy(INA260PowerSensor *vmon){
    REMOVE_VAL_FROM_VEC(bridge_objs, std::shared_ptr<INA260PowerSensor>(vmon));
}

BRIDGE_FUNC double INA260PowerSensor_getCurrent(INA260PowerSensor *vmon){
    return vmon->getCurrent();
}

BRIDGE_FUNC double INA260PowerSensor_getVoltage(INA260PowerSensor *vmon){
    return vmon->getVolgate();
}

BRIDGE_FUNC double INA260PowerSensor_getPower(INA260PowerSensor *vmon){
    return vmon->getPower();
}

BRIDGE_FUNC void INA260PowerSensor_makeMainVmon(INA260PowerSensor *vmon){
    vmon->makeMainVmon();
}

////////////////////////////////////////////////////////////////////////////////
/// StatusLED bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC StatusLED *StatusLED_create(unsigned int pin){
    auto led = std::make_shared<StatusLED>(pin);
    bridge_objs.push_back(led);
    return led.get();
}

BRIDGE_FUNC void StatusLED_destroy(StatusLED *led){
    REMOVE_VAL_FROM_VEC(bridge_objs, std::shared_ptr<StatusLED>(led));
}


////////////////////////////////////////////////////////////////////////////////
/// GPIOPin bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC GPIOPin *GPIOPin_create(unsigned int pin){
    auto dev = std::make_shared<GPIOPin>(pin);
    bridge_objs.push_back(dev);
    return dev.get();
}

BRIDGE_FUNC void GPIOPin_destroy(GPIOPin *dev){
    REMOVE_VAL_FROM_VEC(bridge_objs, std::shared_ptr<GPIOPin>(dev));
}

BRIDGE_FUNC void GPIOPin_setMode(GPIOPin *dev, int mode){
    dev->setMode(static_cast<GPIOPin::Mode>(mode));
}

BRIDGE_FUNC void GPIOPin_setLevel(GPIOPin *dev, int level){
    dev->setLevel(static_cast<GPIOPin::Level>(level));
}

BRIDGE_FUNC int GPIOPin_getLevel(GPIOPin *dev){
    return static_cast<int>(dev->getLevel());
}

BRIDGE_FUNC void GPIOPin_setPwmValue(GPIOPin *dev, uint8_t val){
    dev->setPwmValue(val);
}

BRIDGE_FUNC uint8_t GPIOPin_getPwmValue(GPIOPin *dev){
    return dev->getPwmValue();
}

BRIDGE_FUNC void GPIOPin_setPwmFrequency(GPIOPin *dev, unsigned int freq){
    dev->setPwmFrequency(freq);
}

BRIDGE_FUNC unsigned int GPIOPin_getPwmFrequency(GPIOPin *dev){
    return dev->getPwmFrequency();
}


////////////////////////////////////////////////////////////////////////////////
/// AudioManager bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC size_t AudioManager_getPlaybackDevicesCount(){
    return AudioManager::getPlaybackDevices().size();
}

BRIDGE_FUNC void AudioManager_getPlaybackDevice(size_t index, uint32_t *id, char **name, bool *isDefault, uint8_t *type){
    AudioDeviceInfo info = AudioManager::getPlaybackDevices()[index];
    *id = info.id;
    *name = new char[info.name.length() + 1];
    std::strcpy(*name, info.name.c_str());
    *isDefault = info.isDefault;
    *type = info.type;
}

BRIDGE_FUNC int AudioManager_playSound(const char *filename){
    std::string cppFilename(filename);
    return AudioManager::playSound(cppFilename);
}

BRIDGE_FUNC int AudioManager_playSoundWithDevice(const char *filename, uint32_t playbackDeviceId, const char *playbackDeviceName, bool playbackDeviceIsDefault, uint8_t playbackDeviceType){
    std::string cppFilename(filename);
    AudioDeviceInfo info;
    info.id = playbackDeviceId;
    info.name = playbackDeviceName;
    info.isDefault = playbackDeviceIsDefault;
    info.type = playbackDeviceType;
    return AudioManager::playSound(cppFilename, info);
}

BRIDGE_FUNC void AudioManager_stopJob(int jobId){
    AudioManager::stopJob(jobId);
}


////////////////////////////////////////////////////////////////////////////////
/// PID Bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC PID *PID_create(double kp, double ki, double kd, double kf, double min, double max){
    auto pid = std::make_shared<PID>(kp, ki, kd, kf, min, max);
    bridge_objs.push_back(pid);
    return pid.get();
}

BRIDGE_FUNC void PID_destroy(PID *pid){
    REMOVE_VAL_FROM_VEC(bridge_objs, std::shared_ptr<PID>(pid));
}

BRIDGE_FUNC double PID_getKp(PID *pid){
    return pid->getKp();
}

BRIDGE_FUNC void PID_setKp(PID *pid, double kp){
    pid->setKp(kp);
}

BRIDGE_FUNC double PID_getKi(PID *pid){
    return pid->getKi();
}

BRIDGE_FUNC void PID_setKi(PID *pid, double ki){
    pid->setKi(ki);
}

BRIDGE_FUNC double PID_getKd(PID *pid){
    return pid->getKd();
}

BRIDGE_FUNC void PID_setKd(PID *pid, double kd){
    pid->setKd(kd);
}

BRIDGE_FUNC double PID_getKf(PID *pid){
    return pid->getKf();
}

BRIDGE_FUNC void PID_setKf(PID *pid, double kf){
    pid->setKf(kf);
}

BRIDGE_FUNC double PID_getMin(PID *pid){
    return pid->getMin();
}

BRIDGE_FUNC void PID_setMin(PID *pid, double min){
    pid->setMin(min);
}

BRIDGE_FUNC double PID_getMax(PID *pid){
    return pid->getMax();
}

BRIDGE_FUNC void PID_setMax(PID *pid, double max){
    pid->setMax(max);
}

BRIDGE_FUNC double PID_getSetpoint(PID *pid){
    return pid->getSetpoint();
}

BRIDGE_FUNC void PID_setSetpoint(PID *pid, double setpoint){
    pid->setSetpoint(setpoint);
}

BRIDGE_FUNC void PID_reset(PID *pid){
    pid->reset();
}

BRIDGE_FUNC double PID_getOutput(PID *pid, double currentPv){
    return pid->getOutput(currentPv);
}
