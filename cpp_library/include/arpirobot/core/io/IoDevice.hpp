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

#include <arpirobot/core/io/IoProvider.hpp>
#include <functional>

namespace arpirobot{

    class IoDevice{
    public:
        virtual ~IoDevice();

        void close();

    protected:
        // Close function is assigned by child class.
        // Cannot use virtual func as this is run from destructor.
        IoDevice(std::function<void()> closeFunc);

        std::function<void()> closeFunc;

        friend class IoProvider;
    };

}
