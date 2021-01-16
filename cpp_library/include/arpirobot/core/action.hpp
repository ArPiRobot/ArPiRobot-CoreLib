#pragma once

#include <arpirobot/core/scheduler.hpp>
#include <arpirobot/core/device.hpp>
#include <arpirobot/core/robot.hpp>

#include <vector>
#include <mutex>
#include <memory>

namespace arpirobot{

    class Action{
    public:
        void lockDevices(std::vector<BaseDevice*> devices);

        void lockDevice(BaseDevice *device);

        bool isRunning();

    protected:

        virtual void begin() = 0;

        virtual void process() = 0;

        virtual void finish(bool wasInterrupted) = 0;

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

    
    class ActionManager{
    public:

        /**
         * Start an action
         * @param action    A pointer to the action to start.
         * @param doRestart If true starting an action that is already running will restart the action.
         *                  If false the action will continue running uninterrupted.
         */
        static bool startAction(Action *action, bool doRestart = true);

        /**
         * Add a trigger to start an action when some event occurs.
         * @param trigger A pointer to the trigger to add.
         */
        static void addTrigger(BaseActionTrigger *trigger);

        static void removeTrigger(BaseActionTrigger *trigger);

        static bool stopAction(Action *action);

    private:

        static bool stopActionInternal(Action *action, bool interrupted);

        static void checkTriggers();

        static std::mutex triggerLock;

        static std::vector<BaseActionTrigger*> triggers;

        friend class BaseRobot; // BaseRobot needs to call checkTriggers on its scheduler
        friend class Action;    // Action needs to be able to call stopActionInternal
    };


    class ActionSeries : public Action{
    public:
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