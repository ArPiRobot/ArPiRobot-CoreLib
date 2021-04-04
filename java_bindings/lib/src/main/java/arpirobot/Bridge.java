package arpirobot;

import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.Memory;

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

    /**
     * JNA cannot handle passing pointer array (and converting to void**)
     * However passing pointer to function that takes void** works.
     * This function allocates memory for and creates a Pointer object
     * Containing the data for an array of pointers
     * @param pointerArray The pointer array
     * @return A pointer to the pointer array
     */
    public static Pointer ptrArrayToPtr(Pointer[] pointerArray){
        Memory ptrSingle = new Memory(Native.POINTER_SIZE * pointerArray.length);
        ptrSingle.write(0, pointerArray, 0, pointerArray.length);
        return ptrSingle;
    }

    public static class arpirobot {
        
        static{
            Native.register("arpirobot-core");
        }

        public static native void freeString(Pointer ptr);

        ////////////////////////////////////////////////////////////////////////
        /// BaseRobot bridge
        ////////////////////////////////////////////////////////////////////////
        public static native Pointer BaseRobot_create(void_void_func_ptr robotStarted, void_void_func_ptr robotEnabled, 
                void_void_func_ptr robotDisabled, void_void_func_ptr enabledPeriodic, 
                void_void_func_ptr disabledPeriodic, void_void_func_ptr periodic, 
                int mainSchedulerThreads, int periodicFunctionRate, 
                int maxGamepadDataAge, int actionFunctionPeriod);

        public static native void BaseRobot_destroy(Pointer ptr);

        public static native void BaseRobot_start(Pointer ptr);

        public static native void BaseRobot_feedWatchdog(Pointer ptr);

        ////////////////////////////////////////////////////////////////////////
        /// MainVMon Bridge
        ////////////////////////////////////////////////////////////////////////

        public static native void MainVMon_makeMainVmon(Pointer vmon);

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
        public static native Pointer BaseDevice_getDeviceName(Pointer device);

        
        ////////////////////////////////////////////////////////////////////////
        /// Gamepad bridge
        ////////////////////////////////////////////////////////////////////////
        public static native Pointer Gamepad_create(int controllerNum);

        public static native void Gamepad_destroy(Pointer gamepad);

        public static native int Gamepad_getControllerNum(Pointer gamepad);

        public static native double Gamepad_getAxis(Pointer gamepad, int axisNum, double deadband);

        public static native boolean Gamepad_getButton(Pointer gamepad, int buttonNum);

        public static native int Gamepad_getDpad(Pointer gamepad, int dpadNum);

        public static native void Gamepad_setAxisTransform(Pointer gamepad, int axisNum, Pointer transform);

        public static native void Gamepad_clearAxisTransform(Pointer gamepad, int axisNum);


        ////////////////////////////////////////////////////////////////////////
        /// ButtonPressedTrigger bridge 
        ////////////////////////////////////////////////////////////////////////

        public static native Pointer ButtonPressedTrigger_create(Pointer gamepad, int buttonNum, 
                Pointer targetAction, boolean doRestart);

        public static native void ButtonPressedTrigger_destroy(Pointer trigger);


        ////////////////////////////////////////////////////////////////////////
        /// ButtonReleasedTrigger bridge
        ////////////////////////////////////////////////////////////////////////

        public static native Pointer ButtonReleasedTrigger_create(Pointer gamepad, int buttonNum, 
                Pointer targetAction, boolean doRestart);

        public static native void ButtonReleasedTrigger_destroy(Pointer trigger);

        ////////////////////////////////////////////////////////////////////////
        /// MotorController bridge
        ////////////////////////////////////////////////////////////////////////
        public static native boolean MotorController_isInverted(Pointer motor);

        public static native void MotorController_setInverted(Pointer motor, boolean inverted);

        public static native boolean MotorController_isBrakeMode(Pointer motor);

        public static native void MotorController_setBrakeMode(Pointer motor, boolean brakeMode);

        public static native void MotorController_setSpeed(Pointer motor, double speed);

        public static native double MotorController_getSpeed(Pointer motor);


        ////////////////////////////////////////////////////////////////////////
        /// AdafruitMotorHatMotor bridge (arpirobot/devices/adafruitmotorhat.hpp)
        ////////////////////////////////////////////////////////////////////////

        public static native Pointer AdafruitMotorHatMotor_create(int motorNum, int address, boolean remapNumbers);

        public static native void AdafruitMotorHatMotor_destroy(Pointer motor);


        ////////////////////////////////////////////////////////////////////////
        /// L298Motor bridge (arpirobot/devices/l298n.hpp)
        ////////////////////////////////////////////////////////////////////////

        public static native Pointer L298NMotor_create(int in1Pin, int in2Pin, int pwmPin);

        public static native void L298NMotor_destroy(Pointer motor);


        ////////////////////////////////////////////////////////////////////////
        /// TB6612Motor bridge (arpirobot/devices/tb6612.hpp)
        ////////////////////////////////////////////////////////////////////////

        public static native Pointer TB6612Motor_create(int in1Pin, int in2Pin, int pwmPin);

        public static native void TB6612Motor_destroy(Pointer motor);


        ////////////////////////////////////////////////////////////////////////
        /// DRV8833Motor bridge (arpirobot/devices/drv8833.hpp)
        ////////////////////////////////////////////////////////////////////////

        public static native Pointer DRV8833Motor_create(int in1Pin, int in2Pin, int slpPin);

        public static native void DRV8833Motor_destroy(Pointer motor);


        ////////////////////////////////////////////////////////////////////////
        /// ArcadeDriveHelper bridge
        ////////////////////////////////////////////////////////////////////////
        public static native Pointer ArcadeDriveHelper_create(Pointer leftMotors, int leftMotorCount, 
            Pointer rightMotors, int rightMotorCount);

        public static native void ArcadeDriveHelper_destroy(Pointer helper);

        public static native void ArcadeDriveHelper_updateSpeed(Pointer helper, double speed);

        public static native void ArcadeDriveHelper_updateRotation(Pointer helper, double rotation);

        public static native void ArcadeDriveHelper_update(Pointer helper, double speed, double rotation);


        ////////////////////////////////////////////////////////////////////////
        /// TankDriveHelper bridge
        ////////////////////////////////////////////////////////////////////////

        public static native Pointer TankDriveHelper_create(Pointer leftMotors, int leftMotorCount, 
            Pointer rightMotors, int rightMotorCount);

        public static native void TankDriveHelper_destroy(Pointer helper);

        public static native void TankDriveHelper_updateLeftSpeed(Pointer helper, double newLeftSpeed);

        public static native void TankDriveHelper_updateRightSpeed(Pointer helper, double newRightSpeed);

        public static native void TankDriveHelper_update(Pointer helper, double newLeftSpeed, double newRightSpeed);


        ////////////////////////////////////////////////////////////////////////
        /// BaseAxisTransform bridge
        ////////////////////////////////////////////////////////////////////////
        public static native double BaseAxisTransform_applyTransform(Pointer transform, double rawAxisValue);


        ////////////////////////////////////////////////////////////////////////
        /// SquareRootAxisTransform bridge
        ////////////////////////////////////////////////////////////////////////
        public static native Pointer SquareRootAxisTransform_create();

        public static native void SquareRootAxisTransform_destroy(Pointer transform);


        ////////////////////////////////////////////////////////////////////////
        /// CubicAxisTransform bridge
        ////////////////////////////////////////////////////////////////////////
        public static native Pointer CubicAxisTransform_create(double minPower, double midPower);

        public static native void CubicAxisTransform_destroy(Pointer transform);


        ////////////////////////////////////////////////////////////////////////
        /// Action bridge
        ////////////////////////////////////////////////////////////////////////
        public static native Pointer Action_create(void_void_func_ptr beginPtr,
            void_void_func_ptr processPtr,
            void_bool_func_ptr finishPtr,
            bool_void_func_ptr shouldContinuePtr);

        public static native void Action_destroy(Pointer action);

        public static native void Action_lockDevices(Pointer action, Pointer devices, int deviceCount);

        public static native void Action_lockDevice(Pointer action, Pointer device);

        public static native boolean Action_isRunning(Pointer action);


        ////////////////////////////////////////////////////////////////////////
        /// ActionManager bridge
        ////////////////////////////////////////////////////////////////////////
        public static native boolean ActionManager_startAction(Pointer action);

        public static native boolean ActionManager_stopAction(Pointer action);

        public static native void ActionManager_addTrigger(Pointer trigger);

        public static native void ActionManager_removeTrigger(Pointer trigger);


        ////////////////////////////////////////////////////////////////////////
        /// ActionSeries bridge
        ////////////////////////////////////////////////////////////////////////
        public static native Pointer ActionSeries_create(Pointer actions, int actionCount, Pointer finishAction);

        public static native void ActionSeries_destroy(Pointer actionSeries);


        ////////////////////////////////////////////////////////////////////////
        /// BaseArduinoInterface bridge
        ////////////////////////////////////////////////////////////////////////
        public static native void BaseArduinoInterface_begin(Pointer arduino);

        public static native void BaseArduinoInterface_addDevice(Pointer arduino, Pointer device);

        public static native boolean BaseArduinoInterface_isReady(Pointer arduino);


        ////////////////////////////////////////////////////////////////////////
        /// ArduinoUartInterface bridge
        ////////////////////////////////////////////////////////////////////////
        public static native Pointer ArduinoUartInterface_create(String tty, int baud);

        public static native void ArduinoUartInterface_destroy(Pointer iface);


        ////////////////////////////////////////////////////////////////////////
        /// ArduinoDevice bridge
        ////////////////////////////////////////////////////////////////////////

        public static native Pointer ArduinoDevice_getDeviceName(Pointer device);


        ////////////////////////////////////////////////////////////////////////
        /// VoltageMonitor bridge (arpirobot/arduino/sensor.hpp)
        ////////////////////////////////////////////////////////////////////////

        public static native Pointer VoltageMonitor_create(String pin, double vboard, int r1, 
                int r2, boolean createDevice, int deviceId);

        public static native void VoltageMonitor_destroy(Pointer vmon);

        public static native double VoltageMonitor_getVoltage(Pointer vmon);


        ////////////////////////////////////////////////////////////////////////
        /// Ultrasonic4Pin bridge (arpirobot/arduino/sensor.hpp)
        ////////////////////////////////////////////////////////////////////////

        public static native Pointer Ultrasonic4Pin_create(String triggerPin, String echoPin, 
                boolean createDevice, int deviceId);

        public static native void Ultrasonic4Pin_destroy(Pointer usonic);

        public static native int Ultrasonic4Pin_getDistance(Pointer usonic);


        ////////////////////////////////////////////////////////////////////////
        /// SingleEncode bridge (arpirobot/arduino/sensor.hpp)
        ////////////////////////////////////////////////////////////////////////

        public static native Pointer SingleEncoder_create(String pin, boolean useInternalPullup, 
                boolean createDevice, int deviceId);

        public static native void SingleEncoder_destroy(Pointer enc);

        public static native int SingleEncoder_getPosition(Pointer enc);

        public static native void SingleEncoder_setPosition(Pointer enc, int newPosition);


        ////////////////////////////////////////////////////////////////////////
        /// IRReflectorModule bridge (arpirobot/arduino/sensor.hpp)
        ////////////////////////////////////////////////////////////////////////

        public static native Pointer IRReflectorModule_create(String digitalPin, String analogPin, 
                boolean createDevice, int deviceId);

        public static native void IRReflectorModule_destroy(Pointer ir);

        public static native boolean IRReflectorModule_getDigitalValue(Pointer ir);

        public static native int IRReflectorModule_getAnalogValue(Pointer ir);


        ////////////////////////////////////////////////////////////////////////
        /// OldAdafruit9Dof bridge (arpirobot/arduino/sensor.hpp)
        ////////////////////////////////////////////////////////////////////////

        public static native Pointer OldAdafruit9Dof_create(boolean createDevice, int deviceId);

        public static native void OldAdafruit9Dof_destroy(Pointer imu);

        public static native double OldAdafruit9Dof_getGyroX(Pointer imu);

        public static native double OldAdafruit9Dof_getGyroY(Pointer imu);

        public static native double OldAdafruit9Dof_getGyroZ(Pointer imu);

        public static native double OldAdafruit9Dof_getAccelX(Pointer imu);

        public static native double OldAdafruit9Dof_getAccelY(Pointer imu);

        public static native double OldAdafruit9Dof_getAccelZ(Pointer imu);

        public static native void OldAdafruit9Dof_setGyroX(Pointer imu, double newGyroX);

        public static native void OldAdafruit9Dof_setGyroY(Pointer imu, double newGyroY);

        public static native void OldAdafruit9Dof_setGyroZ(Pointer imu, double newGyroZ);


        ////////////////////////////////////////////////////////////////////////
        /// NxpAdafruit9Dof bridge (arpirobot/arduino/sensor.hpp)
        ////////////////////////////////////////////////////////////////////////

        public static native Pointer NxpAdafruit9Dof_create(boolean createDevice, int deviceId);

        public static native void NxpAdafruit9Dof_destroy(Pointer imu);

        public static native void NxpAdafruit9Dof_calibrate(Pointer imu, int samples);

        public static native double NxpAdafruit9Dof_getGyroX(Pointer imu);

        public static native double NxpAdafruit9Dof_getGyroY(Pointer imu);

        public static native double NxpAdafruit9Dof_getGyroZ(Pointer imu);

        public static native double NxpAdafruit9Dof_getAccelX(Pointer imu);

        public static native double NxpAdafruit9Dof_getAccelY(Pointer imu);

        public static native double NxpAdafruit9Dof_getAccelZ(Pointer imu);

        public static native void NxpAdafruit9Dof_setGyroX(Pointer imu, double newGyroX);

        public static native void NxpAdafruit9Dof_setGyroY(Pointer imu, double newGyroY);

        public static native void NxpAdafruit9Dof_setGyroZ(Pointer imu, double newGyroZ);


        ////////////////////////////////////////////////////////////////////////
        /// Mpu6050Imu bridge (arpirobot/arduino/sensor.hpp)
        ////////////////////////////////////////////////////////////////////////

        public static native Pointer Mpu6050Imu_create(boolean createDevice, int deviceId);

        public static native void Mpu6050Imu_destroy(Pointer imu);

        public static native void Mpu6050Imu_calibrate(Pointer imu, int samples);

        public static native double Mpu6050Imu_getGyroX(Pointer imu);

        public static native double Mpu6050Imu_getGyroY(Pointer imu);

        public static native double Mpu6050Imu_getGyroZ(Pointer imu);

        public static native double Mpu6050Imu_getAccelX(Pointer imu);

        public static native double Mpu6050Imu_getAccelY(Pointer imu);

        public static native double Mpu6050Imu_getAccelZ(Pointer imu);

        public static native void Mpu6050Imu_setGyroX(Pointer imu, double newGyroX);

        public static native void Mpu6050Imu_setGyroY(Pointer imu, double newGyroY);

        public static native void Mpu6050Imu_setGyroZ(Pointer imu, double newGyroZ);


        ////////////////////////////////////////////////////////////////////////
        /// INA260PowerSensor bridge (arpirobot/devices/ina260.hpp)
        ////////////////////////////////////////////////////////////////////////

        public static native Pointer INA260PowerSensor_create();

        public static native void INA260PowerSensor_destroy(Pointer vmon);

        public static native double INA260PowerSensor_getCurrent(Pointer vmon);

        public static native double INA260PowerSensor_getVoltage(Pointer vmon);

        public static native double INA260PowerSensor_getPower(Pointer vmon);

    }


}
