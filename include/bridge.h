#pragma once

#include <core/robot.hpp>

#define BRIDGE_FUNC extern "C"

////////////////////////////////////////////////////////////////////////////////
/// BaseRobot Bridge
////////////////////////////////////////////////////////////////////////////////

BRIDGE_FUNC BaseRobot* start_robot(void (*robotStarted)(void), 
                        void (*robotEnabled)(void), 
                        void (*robotDisabled)(void), 
                        void (*enabledPeriodic)(void), 
                        void (*disabledPeriodic)(void), 
                        void (*periodic)(void),
                        int mainSchedulerThreads,
                        int periodicFunctionRate);

BRIDGE_FUNC void stop_robot(BaseRobot *robot);