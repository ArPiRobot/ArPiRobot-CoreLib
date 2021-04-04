#pragma once

#include <arpirobot/core/device/BaseDevice.hpp>
#include <arpirobot/core/scheduler.hpp>

#include <vector>
#include <mutex>
#include <memory>

namespace arpirobot{

    /**
     * \class Action Action.hpp arpirobot/core/action/Action.hpp
     * 
     * Generic action class. User actions should inherit this class and implement the four pure virtual methods
     */
    class Action{
    public:

        virtual ~Action();

        /**
         * Use this action to lock a set of devices. 
         * This is the same as calling Action::lockDevice once for each device individually.
         * @param devices A vector of devices to lock
         */
        void lockDevices(std::vector<BaseDevice*> devices);

        /**
         * Use this action to lock a device. When a device is locked by this action, 
         * any action previously locking it will be stopped.
         * @param device The device to lock
         */
        void lockDevice(BaseDevice *device);

        /**
         * @return true if the action has been started, but has not finished or been stopped.
         */
        bool isRunning();

    protected:

        /**
         * Run when the action is started.
         */
        virtual void begin() = 0;

        /**
         * Run periodically after the action is started, but before it finishes.
         */
        virtual void process() = 0;

        /**
         * Run when the action is stopped.
         * @param wasInterrupted Will be true if the action did not stop on its own (see Action::shouldContinue)
         */
        virtual void finish(bool wasInterrupted) = 0;

        /**
         * Called each time after Action::process. When this returns false the action will stop.
         * @return false if the action should stop. Else true.
         */
        virtual bool shouldContinue() = 0;

    private:

        void actionStart();

        void actionStop(bool interrupted);

        void actionProcess();

        bool isStarted();
    
        bool isFinished();

        std::shared_ptr<Task> _schedulerTask = nullptr;

        std::mutex stateLock;
        bool started = false;
        bool finished = false;

        friend class ActionManager;
        friend class ActionSeries;
    };

}