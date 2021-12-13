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
    class GPIOPin : public BaseDevice, public IoDevice{
    public:
        enum class Mode {Input = 0, Output = 1};
        enum class Level {Low = 0, High = 1};

        GPIOPin(unsigned int pin);

        GPIOPin(const GPIOPin &other) = delete;

        ~GPIOPin();

        GPIOPin &operator=(const GPIOPin &other) = delete;

        std::string getDeviceName() override;

        // Pin configuration
        void setMode(Mode mode);

        // Digital logic
        void setLevel(Level level);
        Level getLevel();
        
        // PWM
        void setPwmValue(uint8_t val);
        uint8_t getPwmValue();
        void setPwmFrequency(unsigned int freq);
        unsigned int getPwmFrequency();

    protected:
        void close() override;

        void begin() override;

        bool isEnabled() override;

        bool shouldMatchRobotState() override;

        bool shouldDisableWithWatchdog() override;

        void enable() override;

        void disable() override;

    private:
        unsigned int pin;
        uint8_t lastPwmValue;
    };

}