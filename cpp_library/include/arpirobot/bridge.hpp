#pragma once

#include <arpirobot/core/robot.hpp>


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


////////////////////////////////////////////////////////////////////////////////
/// NetworkTable Bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC void NetworkTable_set(const char *key, const char *value);

BRIDGE_FUNC const char *NetworkTable_get(const char *key);

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
