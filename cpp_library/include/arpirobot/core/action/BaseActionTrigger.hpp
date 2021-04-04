#pragma once

#include <arpirobot/core/scheduler.hpp>
#include <arpirobot/core/device/BaseDevice.hpp>
#include <arpirobot/core/robot/BaseRobot.hpp>

#include <vector>
#include <mutex>
#include <memory>

namespace arpirobot{
    /**
     * \class BaseActionTrigger BaseActionTrigger.hpp arpirobot/core/action/BaseActionTrigger.hpp
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
}