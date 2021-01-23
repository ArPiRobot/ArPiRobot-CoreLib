#pragma once

#include <string>
#include <arpirobot/core/device.hpp>

namespace arpirobot{

    /**
     * \class L298NMotor l298n.hpp arpirobot/devices/l298n.hpp
     * 
     * Motor on L298N module
     */
    class L298NMotor : public MotorController{
    public:

        /**
         * @param in1Pin pin for side 1 directional control
         * @param in2Pin pin for side 2 directional control
         * @param pwmPin pin for pwm of motor
         */
        L298NMotor(int in1Pin, int in2Pin, int pwmPin);
        ~L298NMotor();

        /**
         * Get the human-readable name of the device
         * @returns the devices human readable name
         */
        std::string getDeviceName();

    protected:
        void begin();
        
        void run();

    private:
        int in1, in2, pwm;
    };

    /**
     * \class L298NModule l298n.hpp arpirobot/devices/l298n.hpp
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