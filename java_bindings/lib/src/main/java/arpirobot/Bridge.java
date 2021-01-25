package arpirobot;

import com.sun.jna.Native;
import com.sun.jna.Pointer;

import com.sun.jna.Callback;

public class Bridge {

    // Callback for functions taking no args and returning void
    public interface void_void_func_ptr extends Callback{
        public void invoke();
    }

    public static class arpirobot {
        
        static{
            Native.register("arpirobot-core");
        }

        public static native void freeString(Pointer ptr);

        ////////////////////////////////////////////////////////////////////////
        /// BaseRobot bridge
        ////////////////////////////////////////////////////////////////////////
        public static native long BaseRobot_create(void_void_func_ptr robotStarted, void_void_func_ptr robotEnabled, 
                void_void_func_ptr robotDisabled, void_void_func_ptr enabledPeriodic, 
                void_void_func_ptr disabledPeriodic, void_void_func_ptr periodic, 
                int mainSchedulerThreads, int periodicFunctionRate, 
                int maxGamepadDataAge, int actionFunctionPeriod);

        public static native void BaseRobot_destroy(long ptr);

        public static native void BaseRobot_start(long ptr);

        public static native void BaseRobot_feedWatchdog(long ptr);

        ////////////////////////////////////////////////////////////////////////////////
        /// MainVMon Bridge
        ////////////////////////////////////////////////////////////////////////////////

        public static native void MainVMon_makeMainVmon(long vmon);

        ////////////////////////////////////////////////////////////////////////
        /// NetworkTable bridge
        ////////////////////////////////////////////////////////////////////////
        public static native void NetworkTable_set(String key, String value);

        // Return pointer not string so it can be freed
        public static native Pointer NetworkTable_get(String key);

        public static native boolean NetworkTable_has(String key);

        ////////////////////////////////////////////////////////////////////////
        /// Logger bridge
        ////////////////////////////////////////////////////////////////////////
        public static native void Logger_logDebug(String message);

        public static native void Logger_logInfo(String message);

        public static native void Logger_logWarning(String message);

        public static native void Logger_logError(String message);

        public static native void Logger_logDebugFrom(String source, String message);

        public static native void Logger_logInfoFrom(String source, String message);

        public static native void Logger_logWarningFrom(String source, String message);

        public static native void Logger_logErrorFrom(String source, String message);

        public static native void Logger_logNewline();

        ////////////////////////////////////////////////////////////////////////////////
        /// BaseDevice bridge
        ////////////////////////////////////////////////////////////////////////////////
        public static native Pointer BaseDevice_getDeviceName(long device);

        
        ////////////////////////////////////////////////////////////////////////////////
        /// Gamepad bridge
        ////////////////////////////////////////////////////////////////////////////////
        public static native long Gamepad_create(int controllerNum);

        public static native void Gamepad_destroy(long gamepad);

        public static native int Gamepad_getControllerNum(long gamepad);

        public static native double Gamepad_getAxis(long gamepad, int axisNum, double deadband);

        public static native boolean Gamepad_getButton(long gamepad, int buttonNum);

        public static native int Gamepad_getDpad(long gamepad, int dpadNum);

        public static native void Gamepad_setAxisTransform(long gamepad, int axisNum, long transform);

        public static native void Gamepad_clearAxisTransform(long gamepad, int axisNum);

    }


}
