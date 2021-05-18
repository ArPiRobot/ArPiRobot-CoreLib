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

#include <thread>
#include <vector>
#include <string>

#include <arpirobot/arduino/iface/BaseArduinoInterface.hpp>
#include <arpirobot/core/io/IoDevice.hpp>

namespace arpirobot{

    /**
     * \class ArduinoUartInterface ArduinoUartInterface.hpp arpirobot/arduino/iface/ArduinoUartInterface.hpp
     * 
     * Arduino interface implementation using UART to communicate with the arduino.
     */
    class ArduinoUartInterface : public BaseArduinoInterface, public IoDevice{
    public:

        /**
         * @param port The UART port for the arduino (/dev/tty...)
         * @param baud The baud rate for UART communication
         */
        ArduinoUartInterface(std::string port, int baud);

    protected:
        void open() override;
        void close() override;
        bool isOpen() override;
        int available() override;
        uint8_t readOne() override;
        std::vector<uint8_t> readAll() override;
        void write(const uint8_t &b) override;
        std::string getDeviceName() override;
    
    private:
        std::string port;
        int baud;
        int handle = -1;
    };
}