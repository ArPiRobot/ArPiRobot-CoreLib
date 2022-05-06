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

#include <vector>
#include <memory>
#include <functional>

namespace arpirobot{
    /**
     * \class ActionSeries ActionSeries.hpp arpirobot/core/action/ActionSeries.hpp
     * 
     * A special action that will run a sequential set of actions (one at a time)
     */
    class ActionSeries : public Action{
    public:
        /**
         * @param actions A vector of actions to run sequentially (actions must remain in scope for lifetime of ActionSeries)
         * @param finishedAction An action to transition to once other actions are complete (must remain in scope)
         */
        ActionSeries(std::vector<std::reference_wrapper<Action>> actions, Action &finishedAction);

        /**
         * @param actions A vector of actions to run sequentially (shared_ptrs to can use std::make_shared)
         * @param finishedAction An action to transition to once other actions are complete (must remain in scope)
         */
        ActionSeries(std::vector<std::shared_ptr<Action>> actions, Action &finishedAction);

        /**
         * @param actions A vector of actions to run sequentially (actions must remain in scope for lifetime of ActionSeries)
         * @param finishedAction An action to transition to once other actions are complete (can use std::make_shared)
         */
        ActionSeries(std::vector<std::reference_wrapper<Action>> actions, std::shared_ptr<Action> finishedAction);

        /**
         * @param actions A vector of actions to run sequentially (shared_ptrs to can use std::make_shared)
         * @param finishedAction An action to transition to once other actions are complete (can use std::make_shared)
         */
        ActionSeries(std::vector<std::shared_ptr<Action>> actions, std::shared_ptr<Action> finishedAction);

    protected:
        void begin();

        void process();

        void finish(bool interrupted);

        bool shouldContinue();
    
    private:
        std::vector<std::shared_ptr<Action>> actions;
        std::shared_ptr<Action> finishedAction;

        int index = 0;
    };

}