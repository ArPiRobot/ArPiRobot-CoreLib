#pragma once

#include <arpirobot/core/action/Action.hpp>
#include <arpirobot/core/action/BaseActionTrigger.hpp>

#include <vector>
#include <mutex>

namespace arpirobot{
    /**
     * \class ActionManager ActionManager.hpp arpirobot/core/action/ActionManager.hpp
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
         * @return true if the action was started successfully
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
         * @return true if the action was stopped. If false, the action was not running.
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

}