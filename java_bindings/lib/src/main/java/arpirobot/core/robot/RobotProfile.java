package arpirobot.core.robot;

/**
 * Settings to configure general robot behavior
 */
public class RobotProfile {
    /// Number of threads to use on the main scheduler
    public int mainSchedulerThreads = 10;

    /// Rate to run robot periodic functions at (ms)
    public int periodicFunctionRate = 50;

    /// Maximum age gamepad data can be before it is considered invalid (ms)
    public int maxGamepadDataAge = 100;

    /// Rate to run action periodic functions at (ms) 
    public int actionFunctionPeriod = 50;
}
