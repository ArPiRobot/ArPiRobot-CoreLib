#pragma once

#include <arpirobot/core/scheduler.hpp>
#include <arpirobot/core/device.hpp>

#include <vector>
#include <mutex>
#include <memory>

namespace arpirobot{

    class Action{
    public:
        Action();

        void _actionStart();

        void _actionStop();

        void _actionProcess();

        void lockDevices(std::vector<BaseDevice*> devices);

        void lockDevice(BaseDevice *device);

        bool isRunning();

    protected:

        virtual void begin() = 0;

        virtual void process() = 0;

        virtual void finish(bool wasInterrupted) = 0;

        virtual bool shouldContinue() = 0;

    private:
        std::shared_ptr<Task> schedulerTask;
        bool started = false;
        bool finished = false;
    };


    class BaseActionTrigger{
    public:

        // This exists so it is safe to call BaseActionTrigger(std::make_shared<MyAction>()) 
        // (no need to keep reference to action)
        // But if using bridge layer need to use raw pointer
        BaseActionTrigger(std::shared_ptr<Action> *targetAction, bool doRestart = true);
        BaseActionTrigger(Action *targetAction, bool doRestart = true);

        virtual bool shouldRun() = 0;

    private:

        // TODO: Do this with gamepad action transforms
        // Only one of these will be set. If targetAction is nullptr will use shared ptr
        std::shared_ptr<Action> targetActionSharedPtr;
        Action *targetAction = nullptr;
    };

    
    class ActionManager{
    public:
        static void startAction(Action *action, bool doRestart = true);

        static void addTrigger(Action *action);
        static void addTrigger(std::shared_ptr<Action> action);

        static void removeTrigger(Action *action);
        static void removeTrigger(std::shared_ptr<Action> action);

        static void stopAction(Action *action);

    private:

        static void checkTriggers();

        static void stopActionInternal(Action *action, bool interrupted);

        static std::mutex triggerLock;

        // Both of these can be populated. Both will be processed the same way.
        static std::vector<std::shared_ptr<BaseActionTrigger>> triggerSharedPtrs;
        static std::vector<BaseActionTrigger*> triggers;
    };


    class ActionSeries : public Action{
    public:
        // Here for use with bridge (primarily)
        ActionSeries(std::vector<Action*> actions, Action *finishedAction);

        // Generally this should be used from C++ (memory safe, no need to keep instances)
        ActionSeries(std::vector<std::shared_ptr<Action>> actions, std::shared_ptr<Action> finishedAction);

    protected:
        void begin();

        void process();

        void finish(bool interrupted);

        bool shouldContinue();
    
    private:
        std::vector<Action*> actions;
        std::vector<std::shared_ptr<Action>> actionsSharedPtr;

        Action *finishedAction;
        std::shared_ptr<Action> finishedActionSharedPtr;

        int index = 0;
    };

}