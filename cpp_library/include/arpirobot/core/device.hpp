#pragma once

#include <string>
#include <mutex>

namespace arpirobot{

    class BaseDevice{
    public:
        // TODO: Lock device for action API

        void doBegin();

        virtual bool isEnabled() = 0;

        virtual bool shouldMatchRobotState() = 0;

        virtual bool shouldDisableWithWatchdog() = 0;

        virtual void _enable() = 0;

        virtual void _disable() = 0;

        virtual std::string getDeviceName() = 0;
    
    protected:
        virtual void begin() = 0;

        bool initialized = false;
    };

    class MotorController : public BaseDevice{
    public:
        bool isInverted();
        void setInverted(bool inverted);

        bool isBrakeMode();
        void setBrakeMode(bool brakeMode);

        double getSpeed();
        void setSpeed(double speed);

        virtual bool isEnabled();

        virtual bool shouldMatchRobotState();

        virtual bool shouldDisableWithWatchdog();

        virtual void _enable();

        virtual void _disable();

    protected:
        virtual void run() = 0;

        double speed = 0;
        std::mutex lock;
        bool enabled = false;
        bool brakeMode = false;
        int8_t speedFactor = 1; // 1 or -1
    };

}