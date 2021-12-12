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

namespace arpirobot{

    // Forward declare
    class ArduinoDevice;

    /**
     * \class BaseArduinoInterface BaseArduinoInterface.hpp arpirobot/arduino/iface/BaseArduinoInterface.hpp
     * 
     * Common base class (communication protocol agnostic) for an arduino (or other microcontroller) sensor coprocessor.
     * This class handles all the high-level data between the arduino and pi, without implementing a specific
     * communication protocol. See the below classes for different communication protocols.
     */
    class BaseArduinoInterface{
    public:
        virtual ~BaseArduinoInterface();

        BaseArduinoInterface() = default;

        BaseArduinoInterface(const BaseArduinoInterface &other) = delete;
        BaseArduinoInterface &operator=(const BaseArduinoInterface &other) = delete;

        /**
         * Create devices (as needed) and start processing sensor data. After this is called devices can 
         * no longer be added to this arduino using BaseArduinoInterface::addDevice.
         */
        bool begin();

        /**
         * Add a device to this arduino interface instance. 
         * You cannot add devices after BaseArduinoInterface::begin is called
         * @param device The ArduinoDevice instance to add
         */
        void addDevice(ArduinoDevice *device);

        /**
         * Returns true when the arduino is ready to process sensor data (after BaseArduinoInterface::begin 
         * is successful)
         * @return true if ready, else false
         */
        bool isReady();

        /**
         * Send a message from a specific device.
         * THIS SHOULD NOT BE USED FROM USER CODE.
         * Sends a message to the associated device instance on the arduino.
         * @param deviceId The sending/receiving device's ID
         * @param data The data to send
         */
        void sendFromDevice(uint8_t deviceId, std::vector<uint8_t> data);

    protected:
        
        // Communication functions

        void run();

        uint16_t calcCCittFalse(const std::vector<uint8_t> &data, size_t len);

        /**
         * Writes a message to the arduino using proper escape sequences
         * This funcion will throw exceptions from lower level I/O functions
         */
        void writeData(const std::vector<uint8_t> &data);

        /**
         * This method blocks until at least one byte is available. One byte is then read
         * and stored in the workingBuffer. Escape, start, and end sequences are handled
         * appropriately.
         * When a full dataset has been read into the workingBuffer it is moved into the read_dataset.
         * Then, this method return true.
         * Otherwise it returns false
         * This function will throw exceptions from lower level I/O functions
         */
        bool readData();

        /**
         * Using the complete dataset in read_dataset, calculates the CRC and reads the CRC
         * Compares the two
         */
        bool checkData();

        // This function will throw exceptions from lower level I/O functoins
        std::vector<uint8_t> waitForMessage(const std::vector<uint8_t> &prefix, int timeoutMs);

        // Implementation specific (these can and should throw exceptions on errors)
        //     Exceptions thrown should be from arpirobot/core/io/exceptions.hpp
        virtual void open() = 0;
        virtual void close() = 0;
        virtual bool isOpen() = 0;
        virtual int available() = 0;
        virtual uint8_t readOne() = 0;
        virtual std::vector<uint8_t> readAll() = 0;
        virtual void write(const uint8_t &b) = 0;
        virtual std::string getDeviceName() = 0;

    private:

        static bool msgStartsWith(const std::vector<uint8_t> &msg, const std::vector<uint8_t> &prefix);
        static bool msgEquals(const std::vector<uint8_t> &msg1, const std::vector<uint8_t> &msg2);

        std::vector<uint8_t> workingBuffer;
        std::vector<uint8_t> readDataset;
        bool parseStarted = false;
        bool parseEscaped = false;
        std::thread *processThread = nullptr;
        std::vector<ArduinoDevice*> devices;
        bool initialized = false;
        bool arduinoReady = false;

        const static uint8_t START_BYTE;
        const static uint8_t END_BYTE;
        const static uint8_t ESCAPE_BYTE;

        const static std::vector<uint8_t> MSG_START;
        const static std::vector<uint8_t> MSG_ADD;
        const static std::vector<uint8_t> MSG_ADDSUCCESS;

        const static std::vector<uint8_t> CMD_RESET;
        const static std::vector<uint8_t> CMD_END;

        friend class ArduinoDevice;
    };
}