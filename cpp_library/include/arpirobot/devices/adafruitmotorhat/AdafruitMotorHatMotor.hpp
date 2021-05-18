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

#include <arpirobot/core/device/MotorController.hpp>
#include <arpirobot/core/io/IoDevice.hpp>

#include <cstdint>
#include <mutex>
#include <memory>
#include <unordered_map>
#include <vector>

namespace arpirobot{

    namespace internal{
        /**
         * \class AdafruitMotorHat adafruitmotorhatmotor.hpp arpirobot/devices/adafruitmotorhatmotor.hpp
         * 
         * Low level driver to interface with motor hat via I2C
         * arpirobot BaseDevice implementatio is built on top of this
         * Based on https://github.com/adafruit/Adafruit-Motor-HAT-Python-Library/blob/master/Adafruit_MotorHAT/
         */
        class AdafruitMotorHat : public IoDevice{
        public:
            // Motor commands
            enum class MotorCommand {FORWARD, BACKWARD, BRAKE, RELEASE};

            AdafruitMotorHat(uint8_t address, uint8_t bus = 1);
            ~AdafruitMotorHat();

            /**
             * Motor object instantiated by the motor hat
             */
            class LowLevelDCMotor{
            public:
                LowLevelDCMotor(AdafruitMotorHat *hat, int num);

                /**
                 * Set the motor's mode (MotorCommand)
                 * @param cmd The MotorCommand for the motor
                 */
                void run(MotorCommand cmd);

                /**
                 * Set the speed of the motor
                 * @param speed The motor speed (-1.0 to 1.0)
                 */
                void setSpeed(double speed);

            private:
                void kill();

                uint8_t pwm, in1, in2;
                bool canRun = true;
                std::mutex lock;
                AdafruitMotorHat *hat;
            };

            std::shared_ptr<LowLevelDCMotor> getMotor(int index);

        protected:
            void close() override;

        private:
            void startup();


            void setPin(uint8_t pin, bool isHigh);
            void setPWMFreq(int freq);
            void setPWM(uint8_t channel, int on, int off);
            void setAllPWM(int on, int off);     

            int handle = -1;
            std::shared_ptr<LowLevelDCMotor> motors[4];

            // Registers
            const uint8_t __MODE1 = 0x00;
            const uint8_t __MODE2 = 0x01;
            const uint8_t __SUBADR1 = 0x02;
            const uint8_t __SUBADR2 = 0x03;
            const uint8_t __SUBADR3 = 0x04;
            const uint8_t __PRESCALE = 0xFE;
            const uint8_t __LED0_ON_L = 0x06;
            const uint8_t __LED0_ON_H = 0x07;
            const uint8_t __LED0_OFF_L = 0x08;
            const uint8_t __LED0_OFF_H = 0x09;
            const uint8_t __ALL_LED_ON_L = 0xFA;
            const uint8_t __ALL_LED_ON_H = 0xFB;
            const uint8_t __ALL_LED_OFF_L = 0xFC;
            const uint8_t __ALL_LED_OFF_H = 0xFD;

            // Bits
            const uint8_t __RESTART = 0x80;
            const uint8_t __SLEEP = 0x10;
            const uint8_t __ALLCALL = 0x01;
            const uint8_t __INVRT = 0x10;
            const uint8_t __OUTDRV = 0x04;
        };
    }

    /**
     * \class AdafruitMotorHatMotor AdafruitMotorHatMotor.hpp arpirobot/devices/adafruitmotorhatmotor/AdafruitMotorHatMotor.hpp
     * 
     * Motor for Adafruit Motor Hat
     */
    class AdafruitMotorHatMotor : public MotorController{
    public:

        static const int ADAFRUIT_ADDR;
        static const int GEEKWORM_ADDR;
        static const int DETECT_ADDR;

        /**
         * @param motorNum The number for the motor
         * @param address The address of the hat for the motor
         * @param remapNumbers If true the motor numbers will be remapped (if needed) to match the 
         *                     physical order of the Adafruit motor hat
         */
        AdafruitMotorHatMotor(int motorNum, int address = DETECT_ADDR, bool remapNumbers = true);

        /**
         * Get the human-readable name of the device
         * @return the devices human readable name
         */
        std::string getDeviceName();

    protected:
        void begin();
        
        void run();

    private:
        static int remapMotorNumber(int hatAddress, int motorNum);
        static void doDetectAddress();

        static int detectedAddress;
        static std::unordered_map<int, std::shared_ptr<internal::AdafruitMotorHat>> hatMap;

        int motorNum;
        int hatAddress;
        bool remapNumbers;
        std::shared_ptr<internal::AdafruitMotorHat::LowLevelDCMotor> motor;
    };

}