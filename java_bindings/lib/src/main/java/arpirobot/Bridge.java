package arpirobot;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;

import com.sun.jna.Callback;

public class Bridge {

    public static CLibrary arpirobot = (CLibrary)Native.load("arpirobot-core", CLibrary.class);

    // Callback for functions taking no args and returning void
    public interface void_void_func_ptr extends Callback{
        public void invoke();
    }

    public interface CLibrary extends Library {
        
        public void freeString(Pointer ptr);

        ////////////////////////////////////////////////////////////////////////
        /// BaseRobot bridge
        ////////////////////////////////////////////////////////////////////////
        public long BaseRobot_create(void_void_func_ptr robotStarted, void_void_func_ptr robotEnabled, 
                void_void_func_ptr robotDisabled, void_void_func_ptr enabledPeriodic, 
                void_void_func_ptr disabledPeriodic, void_void_func_ptr periodic, 
                int mainSchedulerThreads, int periodicFunctionRate, 
                int maxGamepadDataAge, int actionFunctionPeriod);

        public void BaseRobot_destroy(long ptr);

        public void BaseRobot_start(long ptr);

        public void BaseRobot_feedWatchdog(long ptr);

        ////////////////////////////////////////////////////////////////////////////////
        /// MainVMon Bridge
        ////////////////////////////////////////////////////////////////////////////////

        public void MainVMon_makeMainVmon(long vmon);

        ////////////////////////////////////////////////////////////////////////
        /// NetworkTable bridge
        ////////////////////////////////////////////////////////////////////////
        public void NetworkTable_set(String key, String value);

        // Return pointer not string so it can be freed
        public Pointer NetworkTable_get(String key);

        public boolean NetworkTable_has(String key);

        ////////////////////////////////////////////////////////////////////////
        /// Logger bridge
        ////////////////////////////////////////////////////////////////////////
        public void Logger_logDebug(String message);

        public void Logger_logInfo(String message);

        public void Logger_logWarning(String message);

        public void Logger_logError(String message);

        public void Logger_logDebugFrom(String source, String message);

        public void Logger_logInfoFrom(String source, String message);

        public void Logger_logWarningFrom(String source, String message);

        public void Logger_logErrorFrom(String source, String message);

        public void Logger_logNewline();

        ////////////////////////////////////////////////////////////////////////////////
        /// BaseDevice bridge
        ////////////////////////////////////////////////////////////////////////////////
        public Pointer BaseDevice_getDeviceName(long device);

        
        ////////////////////////////////////////////////////////////////////////////////
        /// Gamepad bridge
        ////////////////////////////////////////////////////////////////////////////////
        public long Gamepad_create(int controllerNum);

        public void Gamepad_destroy(long gamepad);

        public int Gamepad_getControllerNum(long gamepad);

        public double Gamepad_getAxis(long gamepad, int axisNum, double deadband);

        public boolean Gamepad_getButton(long gamepad, int buttonNum);

        public int Gamepad_getDpad(long gamepad, int dpadNum);

        public void Gamepad_setAxisTransform(long gamepad, int axisNum, long transform);

        public void Gamepad_clearAxisTransform(long gamepad, int axisNum);

    }


}
