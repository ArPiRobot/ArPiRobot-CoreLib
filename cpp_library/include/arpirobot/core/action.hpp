#pragma once

#include <arpirobot/core/scheduler.hpp>
#include <arpirobot/core/device.hpp>
#include <arpirobot/core/robot.hpp>

#include <vector>
#include <mutex>
#include <memory>

namespace arpirobot{

    /**
     * \class Action action.hpp arpirobot/core/action.hpp
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
         * @returns true if the action has been started, but has not finished or been stopped.
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
         * @returns false if the action should stop. Else true.
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

    /**
     * \class BaseActionTrigger action.hpp arpirobot/core/action.hpp
     * 
     * Generic action trigger. Triggers are registered with the action manager. When some event occurs
     * a trigger will run the designated action.
     */
    class BaseActionTrigger{
    public:
        BaseActionTrigger(Action *targetAction, bool doRestart = true);

    protected:
        virtual bool shouldRun() = 0;

    private:

        void startTargetAction();

        Action *targetAction = nullptr;
        bool doRestart = true;

        friend class ActionManager;
    };


    /**
     * \class ActionManager action.hpp arpirobot/core/action.hpp
     * 
     * Static helper class to manage actions and triggers
     */
    class ActionManager{
    public:

        /**
         * Start an action
         * @param action    A pointer to the action to start.
         * @param doRestart If true starting an action that is already running will restart the action.
         *                  If false the action will continue running uninterrupted.
         * @returns true if the action was started successfully
         */
        static bool startAction(Action *action, bool doRestart = true);

        /**
         * Add a trigger to start an action when some event occurs.
         * @param trigger A pointer to the trigger to add.
         */
        static void addTrigger(BaseActionTrigger *trigger);

        /**
         * Remove a trigger
         * @param trigger A pointer to the trigger to remove
         */
        static void removeTrigger(BaseActionTrigger *trigger);

        /**
         * Stop an action (interrupts it)
         * If the action is not running nothing is done.
         * @param action A pointer to the action to stop
         * @returns true if the action was stopped. If false, the action was not running.
         */
        static bool stopAction(Action *action);

    private:

        static bool stopActionInternal(Action *action, bool interrupted);

        static void checkTriggers();

        static std::mutex triggerLock;

        static std::vector<BaseActionTrigger*> triggers;

        friend class BaseRobot; // BaseRobot needs to call checkTriggers on its scheduler
        friend class Action;    // Action needs to be able to call stopActionInternal
    };


    /**
     * \class ActionSeries action.hpp arpirobot/core/action.hpp
     * 
     * A special action that will run a sequential set of actions (one at a time)
     */
    class ActionSeries : public Action{
    public:
        /**
         * @param actions A vector of actions to run sequentially
         * @param finishedAction An action to transition to once other actions are complete
         */
        ActionSeries(std::vector<Action*> actions, Action *finishedAction);

    protected:
        void begin();

        void process();

        void finish(bool interrupted);

        bool shouldContinue();
    
    private:
        std::vector<Action*> actions;
        Action *finishedAction;

        int index = 0;
    };

}