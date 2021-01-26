#pragma once

#include <string>
#include <mutex>

namespace arpirobot{

    class Action; // Forward declare b/c this header is included by action.hpp

    /**
     * \class BaseDevice device.hpp arpirobot/core/device.hpp
     * 
     * Base class for any device connected to the raspberry pi.
     */
    class BaseDevice{
    public:

        virtual ~BaseDevice();

        /**
         * Get the human-readable name of the device
         * @return the devices human readable name
         */
        virtual std::string getDeviceName() = 0;

    protected:
        virtual void begin() = 0;
        virtual bool isEnabled() = 0;
        virtual bool shouldMatchRobotState() = 0;
        virtual bool shouldDisableWithWatchdog() = 0;
        virtual void enable() = 0;
        virtual void disable() = 0;
    
        bool initialized = false;

    private:

        void lockDevice(Action *action);

        bool isLockedByAction();

        void doBegin();

        std::mutex actionLock;
        Action *lockingAction = nullptr;

        friend class Action; // Needs to call lockDevice
        friend class BaseRobot; // Needs doBegin, enable, disable
    };

    /**
     * \class MotorController device.hpp arpirobot/core/device.hpp
     * 
     * Base class for motor controllers.
     */
    class MotorController : public BaseDevice{
    public:

        virtual ~MotorController();

        /**
         * Check if the motor direction is inverted (positive and negative speed switched)
         * @return true if inverted, else false
         */
        bool isInverted();

        /**
         * Set the motor direction to inverted or non-inverted (positive and negative speed switched)
         * @param inverted True for inverted, false for non-inverted
         */
        void setInverted(bool inverted);

        /**
         * Check if brake mode is enabled. When enabled motors with a speed of zero will resist motion.
         * @return true if enabled, else false
         */
        bool isBrakeMode();

        /**
         * Set if brake mode is enabled
         * @param brakeMode true for enabled, false for disabled
         */
        void setBrakeMode(bool brakeMode);

        /**
         * Get the current speed of the motor
         * @return the motor's speed
         */
        double getSpeed();

        /**
         * Set the current speed of the motor (no effect if motor is disabled)
         * @param speed The motor's speed (between -1.0 and 1.0)
         */
        void setSpeed(double speed);

    protected:
        bool isEnabled();

        bool shouldMatchRobotState();

        bool shouldDisableWithWatchdog();

        void enable();

        void disable();
        
        virtual void run() = 0;

        double speed = 0;
        std::mutex lock;
        bool enabled = false;
        bool brakeMode = false;
        int8_t speedFactor = 1; // 1 or -1
    };

}