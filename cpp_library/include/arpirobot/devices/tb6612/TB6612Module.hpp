#pragma once

#include <arpirobot/devices/tb6612/TB6612Motor.hpp>

namespace arpirobot{
    /**
     * \class TB6612Module tb6612.hpp arpirobot/devices/tb6612.hpp
     * 
     * TB6612 Module (two motors)
     */
    class TB6612Module{
    public:

        /**
         * @param aIn1Pin In1 pin for motor a of module
         * @param aIn2Pin In2 pin for motor a of module
         * @param pwmaPin PWM pin for motor a of module
         * @param bIn1Pin In1 pin for motor b of module
         * @param bIn2Pin In2 pin for motor b of module
         * @param pwmbPin PWM pin for motor b of module
         */
        TB6612Module(int aIn1Pin, int aIn2Pin, int pwmaPin, int bIn1Pin, int bIn2Pin, int pwmbPin);

        /**
         * Get the Motor instance for motor a
         */
        TB6612Motor &getMotorA();

        /**
         * Get the Motor instance for motor b
         */
        TB6612Motor &getMotorB();

    private:
        TB6612Motor motorA, motorB;
    };
}