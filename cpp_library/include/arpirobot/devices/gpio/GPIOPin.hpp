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
     * \class GPIOPin GPIOPin.hpp arpirobot/devices/gpio/GPIOPin.hpp
     * Use this device to directly control a GPIO pin
     * This allows using the pin either as an input or an output
     * Supported operations:
     * - Digital Write & Read
     * - PWM Value (0-255)
     * - PWM Frequency (Hz)
     */
    class GPIOPin : public BaseDevice, public IoDevice{
    public:
        enum class Mode {Input = 0, Output = 1};
        enum class Level {Low = 0, High = 1};

        GPIOPin(unsigned int pin);

        GPIOPin(const GPIOPin &other) = delete;

        ~GPIOPin();

        GPIOPin &operator=(const GPIOPin &other) = delete;

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