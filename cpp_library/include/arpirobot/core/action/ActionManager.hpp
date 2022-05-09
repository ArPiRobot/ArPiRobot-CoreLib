/*
 * Copyright 2021 Marcus Behel
 *
 * This file is part of ArPiRobot-CoreLib.
 * 
 * ArPiRobot-CoreLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ArPiRobot-CoreLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with ArPiRobot-CoreLib.  If not, see <https://www.gnu.org/licenses/>. 
 */

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
         * @param action    A reference to the action to start. Referenced action must remain in scope while action is running
         * @param doRestart If true starting an action that is already running will restart the action.
         *                  If false the action will continue running uninterrupted.
         * @return true if the action was started successfully
         */
        static bool startAction(Action &action, bool doRestart = true);

        /**
         * Start an action
         * @param action    A shared_ptr to the action to start. Can use with std::make_shared
         * @param doRestart If true starting an action that is already running will restart the action.
         *                  If false the action will continue running uninterrupted.
         * @return true if the action was started successfully
         */
        static bool startAction(std::shared_ptr<Action> action, bool doRestart = true);

        /**
         * Stop an action (interrupts it)
         * If the action is not running nothing is done.
         * @param action A reference to the action to stop. Referenced action must remain in scope while action is running
         * @return true if the action was stopped. If false, the action was not running.
         */
        static bool stopAction(Action &action);

        /**
         * Stop an action (interrupts it)
         * If the action is not running nothing is done.
         * @param action A shared_ptr to the action to stop. Can use with std::make_shared
         * @return true if the action was stopped. If false, the action was not running.
         */
        static bool stopAction(std::shared_ptr<Action> action);

        /**
         * Add a trigger to start an action when some event occurs.
         * @param trigger A reference to trigger to add. Must remain in scope until removed.
         */
        static void addTrigger(BaseActionTrigger &trigger);

        /**
         * Add a trigger to start an action when some event occurs.
         * @param trigger A shared_ptr to the trigger to add. Can use with std::make_shared
         */
        static void addTrigger(std::shared_ptr<BaseActionTrigger> trigger);

        /**
         * Remove a trigger
         * @param trigger A reference to trigger to remove.
         */
        static void removeTrigger(BaseActionTrigger &trigger);

        /**
         * Remove a trigger
         * @param trigger A shared_ptr to the trigger to remove
         */
        static void removeTrigger(std::shared_ptr<BaseActionTrigger> trigger);

    private:

        static bool stopActionInternal(std::shared_ptr<Action> action, bool interrupted);

        static void checkTriggers();

        static std::mutex triggerLock;

        static std::vector<std::shared_ptr<BaseActionTrigger>> triggers;

        // Keep actions started with ActionManager::startAction(std::make_shared) in scope
        static std::vector<std::shared_ptr<Action>> runningActions;
        static std::mutex runningActionsLock;

        friend class BaseRobot; // BaseRobot needs to call checkTriggers on its scheduler
        friend class Action;    // Action needs to be able to call stopActionInternal
    };

}