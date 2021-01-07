#pragma once

#include <arpirobot/core/robot.hpp>
#include <arpirobot/core/log.hpp>

using namespace arpirobot;

#define BRIDGE_FUNC extern "C"


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
