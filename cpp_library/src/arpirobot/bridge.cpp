#include <arpirobot/bridge.hpp>


////////////////////////////////////////////////////////////////////////////////
/// BaseRobot Bridge
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
                        int periodicFunctionRate){
    RobotProfile profile;

    profile.mainSchedulerThreads = mainSchedulerThreads;
    profile.periodicFunctionRate = periodicFunctionRate;

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