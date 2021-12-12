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

    namespace internal{
        /**
         * Adafruit INA260 module internal driver
         */
        class AdafruitINA260 : public IoDevice{
        public:
            /**
             * @param address Address for the INA260 (I2C)
             * @param bus I2C bus
             */
            AdafruitINA260(int address, int bus = 1);

            AdafruitINA260(const AdafruitINA260 &other) = delete;

            ~AdafruitINA260();

            AdafruitINA260 &operator=(const AdafruitINA260 &other) = delete;

            bool begin();

            void reset();

            // mA
            double readCurrent();

            // mW
            double readPower();


            // mV
            double readBusVoltage();

            void setMode(int mode);

            int getMode();

            void setAveragingCount(int count);

            int getAveragingCount();

            void setCurrentConversionTime(int time);

            int getCurrentConversionTime();

            void setVoltageConversionTime(int time);

            int getVoltageConversiontime();

            bool conversionReady();


            // INA260 Mode values
            const static int MODE_SHUTDOWN;
            const static int MODE_TRIGGERED;
            const static int MODE_CONTINUOUS;

            // INA260 Conversion Time values
            const static int TIME_140_US;
            const static int TIME_204_US;
            const static int TIME_332_US;
            const static int TIME_558_US;
            const static int TIME_1_1_MS;
            const static int TIME_2_116_MS;
            const static int TIME_4_156_MS;
            const static int TIME_8_244_MS;

            // INA260 Averaging Count values
            const static int COUNT_1;
            const static int COUNT_2;
            const static int COUNT_16;
            const static int COUNT_64;
            const static int COUNT_128;
            const static int COUNT_256;
            const static int COUNT_512;
            const static int COUNT_1024;

        protected:

            void close() override;

        private:
            int i2cReadWordHelper(int reg);

            void i2cWriteWordHelper(int reg, int data);

            // Registers
            const static int REG_CONFIG;
            const static int REG_CURRENT;
            const static int REG_BUSVOLTAGE;
            const static int REG_POWER;
            const static int REG_MASK_ENABLE;
            const static int REG_ALERT_LIMIT;
            const static int REG_MFG_UID;
            const static int REG_DIE_UID;

            int handle = -1;
        };
    }

    /**
     * \class INA260PowerSensor INA260PowerSensor.hpp arpirobot/devices/ina260/INA260PowerSensor.hpp
     * INA260 Power sensor
     */
    class INA260PowerSensor : public BaseDevice, public MainVmon{
    public:
        INA260PowerSensor();

        INA260PowerSensor(const INA260PowerSensor &other) = delete;

        ~INA260PowerSensor();

        INA260PowerSensor &operator=(const INA260PowerSensor &other) = delete;

        std::string getDeviceName() override;

        /**
         * Get current measurement
         * @return Current in mA
         */
        double getCurrent();

        /**
         * Get voltage measurement
         * @return Voltage in V
         */
        double getVolgate();

        /**
         * Get power measurement
         * @return Power in mW
         */
        double getPower();

    protected:
        void begin() override;

        bool isEnabled() override;

        bool shouldMatchRobotState() override;

        bool shouldDisableWithWatchdog() override;

        void enable() override;

        void disable() override;

    private:

        void feed();

        std::shared_ptr<internal::AdafruitINA260> sensor;

        double current = 0, voltage = 0, power = 0;

        bool stop = false;
    };

}