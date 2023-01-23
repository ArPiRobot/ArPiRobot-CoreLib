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

#include <arpirobot/core/device/BaseDevice.hpp>
#include <arpirobot/core/scheduler.hpp>
#include <arpirobot/core/network/MainVmon.hpp>
#include <arpirobot/core/io/IoDevice.hpp>

namespace arpirobot{

    /**
     * \class StatusLED StatusLED.hpp arpirobot/devices/gpio/StatusLED.hpp
     * Controls an LED connected to a GPIO pin to add a visual indicator of robot status.
     * While a robot program is running, the LED indicates the robot's state.
     * While the robot is disabled, the LED is solid.
     * While the robot is enabled, the LED blinks.
     */
    class StatusLED : public BaseDevice, public IoDevice{
    public:
        StatusLED(unsigned int pin);

        StatusLED(const StatusLED &other) = delete;

        ~StatusLED();

        StatusLED &operator=(const StatusLED &other) = delete;

    protected:
        void close() override;

        void begin() override;

        bool isEnabled() override;

        bool shouldMatchRobotState() override;

        bool shouldDisableWithWatchdog() override;

        void enable() override;

        void disable() override;

    private:
        void blink();
        std::shared_ptr<Task> schedulerTask = nullptr;

        unsigned int pin;
    };

}