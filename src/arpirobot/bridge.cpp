#include <arpirobot/bridge.h>


BRIDGE_FUNC BaseRobot* BaseRobot_create(void (*robotStarted)(void), 
                        void (*robotEnabled)(void), 
                        void (*robotDisabled)(void), 
                        void (*enabledPeriodic)(void), 
                        void (*disabledPeriodic)(void), 
                        void (*periodic)(void),
                        int mainSchedulerThreads,
                        int periodicFunctionRate){
    RobotProfile profile;
    RobotCallbacks callbacks;

    callbacks.robotStarted = robotStarted;
    callbacks.robotEnabled = robotEnabled;
    callbacks.robotDisabled = robotDisabled;
    callbacks.enabledPeriodic = enabledPeriodic;
    callbacks.disabledPeriodic = disabledPeriodic;
    callbacks.periodic = periodic;

    profile.mainSchedulerThreads = mainSchedulerThreads;
    profile.periodicFunctionRate = periodicFunctionRate;

    BaseRobot *robot = new BaseRobot(callbacks, profile);
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