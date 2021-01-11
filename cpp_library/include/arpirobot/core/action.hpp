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
        // NOT TO BE CALLED BY USER CODE
        void _actionStart();

        // NOT TO BE CALLED BY USER CODE
        void _actionStop(bool interrupted);

        // NOT TO BE CALLED BY USER CODE
        void _actionProcess();

        void lockDevices(std::vector<BaseDevice*> devices);

        void lockDevice(BaseDevice *device);

        bool isRunning();

        // TODO: Move these private and make ActionManager friend class
        bool isStarted();
    
        bool isFinished();

        std::shared_ptr<Task> _schedulerTask = nullptr;

    protected:

        virtual void begin() = 0;

        virtual void process() = 0;

        virtual void finish(bool wasInterrupted) = 0;

        virtual bool shouldContinue() = 0;

    private:
        std::mutex stateLock;
        bool started = false;
        bool finished = false;
    };


    class BaseActionTrigger{
    public:
        BaseActionTrigger(Action *targetAction, bool doRestart = true);

        virtual bool shouldRun() = 0;

        void startTargetAction();

    private:

        Action *targetAction = nullptr;
        bool doRestart = true;
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

        // NOT TO BE CALLED FROM USER CODE
        static bool _stopActionInternal(Action *action, bool interrupted);

    private:

        static void checkTriggers();

        static std::mutex triggerLock;

        static std::vector<BaseActionTrigger*> triggers;

        // BaseRobot needs to call checkTriggers on its scheduler
        friend class BaseRobot;
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