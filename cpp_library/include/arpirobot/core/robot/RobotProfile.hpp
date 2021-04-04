#pragma once

namespace arpirobot{

    /**
     * \class RobotProfile RobotProfile.hpp arpirobot/core/robot/RobotProfile.hpp
     * 
     * Settings to configure general robot behavior
     */
    struct RobotProfile{
        /// Number of threads to use on the main scheduler
        int mainSchedulerThreads = 10; 

        /// Rate to run robot periodic functions at (ms)
        int periodicFunctionRate = 50; 

        /// Maximum age gamepad data can be before it is considered invalid (ms)
        int maxGamepadDataAge = 100;  
        
        /// Rate to run action periodic functions at (ms) 
        int actionFunctionPeriod = 50; 
    };
}