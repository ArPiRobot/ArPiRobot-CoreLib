#pragma once

#include <string>
#include <arpirobot/core/device/MotorController.hpp>

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
         * @return the devices human readable name
         */
        std::string getDeviceName();

    protected:
        void begin();
        
        void run();

    private:
        int in1, in2, pwm;
    };
}