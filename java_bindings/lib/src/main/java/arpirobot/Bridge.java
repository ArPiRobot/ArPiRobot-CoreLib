package arpirobot;

import com.sun.jna.Native;
import com.sun.jna.Pointer;

import com.sun.jna.Callback;

public class Bridge {

    // Callback for functions taking no args and returning void
    public interface void_void_func_ptr extends Callback{
        public void invoke();
    }

    // Callback for functions taking no args and returning bool
    public interface bool_void_func_ptr extends Callback {
        public boolean invoke();
    }

    // Callback for functions taking args (bool) and returning void
    public interface void_bool_func_ptr extends Callback {
        public void invoke(boolean arg);
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

        ////////////////////////////////////////////////////////////////////////
        /// MainVMon Bridge
        ////////////////////////////////////////////////////////////////////////

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

        ////////////////////////////////////////////////////////////////////////
        /// BaseDevice bridge
        ////////////////////////////////////////////////////////////////////////
        public static native Pointer BaseDevice_getDeviceName(long device);

        
        ////////////////////////////////////////////////////////////////////////
        /// Gamepad bridge
        ////////////////////////////////////////////////////////////////////////
        public static native long Gamepad_create(int controllerNum);

        public static native void Gamepad_destroy(long gamepad);

        public static native int Gamepad_getControllerNum(long gamepad);

        public static native double Gamepad_getAxis(long gamepad, int axisNum, double deadband);

        public static native boolean Gamepad_getButton(long gamepad, int buttonNum);

        public static native int Gamepad_getDpad(long gamepad, int dpadNum);

        public static native void Gamepad_setAxisTransform(long gamepad, int axisNum, long transform);

        public static native void Gamepad_clearAxisTransform(long gamepad, int axisNum);


        ////////////////////////////////////////////////////////////////////////
        /// ButtonPressedTrigger bridge 
        ////////////////////////////////////////////////////////////////////////

        public static native long ButtonPressedTrigger_create(long gamepad, int buttonNum, 
                long targetAction, boolean doRestart);

        public static native void ButtonPressedTrigger_destroy(long trigger);


        ////////////////////////////////////////////////////////////////////////
        /// ButtonReleasedTrigger bridge
        ////////////////////////////////////////////////////////////////////////

        public static native long ButtonReleasedTrigger_create(long gamepad, int buttonNum, 
                long targetAction, boolean doRestart);

        public static native void ButtonReleasedTrigger_destroy(long trigger);

        ////////////////////////////////////////////////////////////////////////
        /// MotorController bridge
        ////////////////////////////////////////////////////////////////////////
        public static native boolean MotorController_isInverted(long motor);

        public static native void MotorController_setInverted(long motor, boolean inverted);

        public static native boolean MotorController_isBrakeMode(long motor);

        public static native void MotorController_setBrakeMode(long motor, boolean brakeMode);

        public static native void MotorController_setSpeed(long motor, double speed);

        public static native double MotorController_getSpeed(long motor);


        ////////////////////////////////////////////////////////////////////////
        /// AdafruitMotorHatMotor bridge (arpirobot/devices/adafruitmotorhat.hpp)
        ////////////////////////////////////////////////////////////////////////

        public static native long AdafruitMotorHatMotor_create(int motorNum, int address, boolean remapNumbers);

        public static native void AdafruitMotorHatMotor_destroy(long motor);


        ////////////////////////////////////////////////////////////////////////
        /// L298Motor bridge (arpirobot/devices/l298n.hpp)
        ////////////////////////////////////////////////////////////////////////

        public static native long L298NMotor_create(int in1Pin, int in2Pin, int pwmPin);

        public static native void L298NMotor_destroy(long motor);


        ////////////////////////////////////////////////////////////////////////
        /// TB6612Motor bridge (arpirobot/devices/tb6612.hpp)
        ////////////////////////////////////////////////////////////////////////

        public static native long TB6612Motor_create(int in1Pin, int in2Pin, int pwmPin);

        public static native void TB6612Motor_destroy(long motor);


        ////////////////////////////////////////////////////////////////////////
        /// DRV8833Motor bridge (arpirobot/devices/drv8833.hpp)
        ////////////////////////////////////////////////////////////////////////

        public static native long DRV8833Motor_create(int in1Pin, int in2Pin, int slpPin);

        public static native void DRV8833Motor_destroy(long motor);


        ////////////////////////////////////////////////////////////////////////
        /// ArcadeDriveHelper bridge
        ////////////////////////////////////////////////////////////////////////
        public static native long ArcadeDriveHelper_create(long[] leftMotors, int leftMotorCount, 
            long[] rightMotors, int rightMotorCount);

        public static native void ArcadeDriveHelper_destroy(long helper);

        public static native void ArcadeDriveHelper_updateSpeed(long helper, double speed);

        public static native void ArcadeDriveHelper_updateRotation(long helper, double rotation);

        public static native void ArcadeDriveHelper_update(long helper, double speed, double rotation);


        ////////////////////////////////////////////////////////////////////////
        /// TankDriveHelper bridge
        ////////////////////////////////////////////////////////////////////////

        public static native long TankDriveHelper_create(long[] leftMotors, int leftMotorCount, 
            long[] rightMotors, int rightMotorCount);

        public static native void TankDriveHelper_destroy(long helper);

        public static native void TankDriveHelper_updateLeftSpeed(long helper, double newLeftSpeed);

        public static native void TankDriveHelper_updateRightSpeed(long helper, double newRightSpeed);

        public static native void TankDriveHelper_update(long helper, double newLeftSpeed, double newRightSpeed);


        ////////////////////////////////////////////////////////////////////////
        /// BaseAxisTransform bridge
        ////////////////////////////////////////////////////////////////////////
        public static native double BaseAxisTransform_applyTransform(long transform, double rawAxisValue);


        ////////////////////////////////////////////////////////////////////////
        /// SquareRootAxisTransform bridge
        ////////////////////////////////////////////////////////////////////////
        public static native long SquareRootAxisTransform_create();

        public static native void SquareRootAxisTransform_destroy(long transform);


        ////////////////////////////////////////////////////////////////////////
        /// CubicAxisTransform bridge
        ////////////////////////////////////////////////////////////////////////
        public static native long CubicAxisTransform_create(double minPower, double midPower);

        public static native void CubicAxisTransform_destroy(long transform);


        ////////////////////////////////////////////////////////////////////////
        /// Action bridge
        ////////////////////////////////////////////////////////////////////////
        public static native long Action_create(void_void_func_ptr beginPtr,
            void_void_func_ptr processPtr,
            void_bool_func_ptr finishPtr,
            bool_void_func_ptr shouldContinuePtr);

        public static native void Action_destroy(long action);

        public static native void Action_lockDevices(long action, long[] devices, int deviceCount);

        public static native void Action_lockDevice(long action, long device);

        public static native boolean Action_isRunning(long action);


        ////////////////////////////////////////////////////////////////////////
        /// ActionManager bridge
        ////////////////////////////////////////////////////////////////////////
        public static native boolean ActionManager_startAction(long action);

        public static native boolean ActionManager_stopAction(long action);

        public static native void ActionManager_addTrigger(long trigger);

        public static native void ActionManager_removeTrigger(long trigger);


        ////////////////////////////////////////////////////////////////////////
        /// ActionSeries bridge
        ////////////////////////////////////////////////////////////////////////
        public static native long ActionSeries_create(long[] actions, int actionCount, long finishAction);

        public static native void ActionSeries_destroy(long actionSeries);

    }


}
