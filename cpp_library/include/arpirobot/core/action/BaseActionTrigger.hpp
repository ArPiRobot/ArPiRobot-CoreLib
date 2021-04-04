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