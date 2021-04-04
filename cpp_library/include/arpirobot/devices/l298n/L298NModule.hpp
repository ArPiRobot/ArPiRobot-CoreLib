#pragma once

#include <arpirobot/devices/l298n/L298NMotor.hpp>

namespace arpirobot{
    /**
     * \class L298NModule L298NModule.hpp arpirobot/devices/L298NModule.hpp
     * 
     * L298N Module (two motors)
     */
    class L298NModule{
    public:

        /**
         * @param in1Pin In1 pin for module (motor a)
         * @param in2Pin In2 pin for module (motor a)
         * @param enaPin PWM pin for motor a of module
         * @param in3Pin In3 pin for module (motor b)
         * @param in4Pin In4 pin for module (motor b)
         * @param enbPin PWM pin for motor b of module
         */
        L298NModule(int in1Pin, int in2Pin, int enaPin, int in3Pin, int in4Pin, int enbPin);

        /**
         * Get the Motor instance for motor a
         */
        L298NMotor &getMotorA();

        /**
         * Get the Motor instance for motor b
         */
        L298NMotor &getMotorB();

    private:
        L298NMotor motorA, motorB;
    };
}