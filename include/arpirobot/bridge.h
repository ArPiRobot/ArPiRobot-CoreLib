#pragma once

#include <arpirobot/core/robot.hpp>
#include <arpirobot/core/log.hpp>

using namespace arpirobot;

#define BRIDGE_FUNC extern "C"


////////////////////////////////////////////////////////////////////////////////
/// BaseRobot Bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC BaseRobot* BaseRobot_create(void (*robotStarted)(void), 
                        void (*robotEnabled)(void), 
                        void (*robotDisabled)(void), 
                        void (*enabledPeriodic)(void), 
                        void (*disabledPeriodic)(void), 
                        void (*periodic)(void),
                        int mainSchedulerThreads,
                        int periodicFunctionRate);

BRIDGE_FUNC void BaseRobot_destroy(BaseRobot *robot);

BRIDGE_FUNC void BaseRobot_feedWatchdog(BaseRobot *robot);
