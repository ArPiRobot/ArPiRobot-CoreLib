#pragma once

#include <string>
#include <mutex>

namespace arpirobot{

    class Action; // Forward declare b/c this header is included by action.hpp

    class BaseDevice{
    public:
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

    class MotorController : public BaseDevice{
    public:
        bool isInverted();
        void setInverted(bool inverted);

        bool isBrakeMode();
        void setBrakeMode(bool brakeMode);

        double getSpeed();
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