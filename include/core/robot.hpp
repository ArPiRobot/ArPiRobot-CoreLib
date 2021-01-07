#pragma once

#include <Scheduler.h>

struct RobotProfile{
    int mainSchedulerThreads = 10;
    int periodicFunctionRate = 50;
};

struct RobotCallbacks{
    void (*robotStarted)(void);
    void (*robotEnabled)(void);
    void (*robotDisabled)(void);
    void (*enabledPeriodic)(void);
    void (*disabledPeriodic)(void);
    void (*periodic)(void);
};

class BaseRobot{
public:
    BaseRobot(RobotCallbacks callbacks, RobotProfile profile);

private:
    void modeBasedPeriodic();

    RobotProfile profile;
    RobotCallbacks callbacks;
    Bosma::Scheduler scheduler;
    bool isEnabled = false;
};