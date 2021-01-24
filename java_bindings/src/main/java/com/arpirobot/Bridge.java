package com.arpirobot;

import com.arpirobot.core.BaseRobot;

public class Bridge {
    ////////////////////////////////////////////////////////////////////////////
    /// BaseRobot Bridge
    ////////////////////////////////////////////////////////////////////////////
    static native long BaseRobot_create(BaseRobot javaRobotObj);
    static native void BaseRobot_destroy(long ptr);
    static native void BaseRobot_start(long ptr);
    static native void BaseRobot_feedWatchdog(long ptr);
}
