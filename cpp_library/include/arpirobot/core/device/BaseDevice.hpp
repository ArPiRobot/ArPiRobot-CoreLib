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
}