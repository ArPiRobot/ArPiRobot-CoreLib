#pragma once

#include <arpirobot/core/device/MotorController.hpp>

namespace arpirobot{

    /**
     * \class TB6612Motor TB6612Motor.hpp arpirobot/devices/TB6612Motor.hpp
     * 
     * Motor on TB6612 module
     */
    class TB6612Motor : public MotorController{
    public:
        /**
         * @param in1Pin pin for side 1 directional control
         * @param in2Pin pin for side 2 directional control
         * @param pwmPin pin for pwm of motor
         */
        TB6612Motor(int in1Pin, int in2Pin, int pwmPin);
        ~TB6612Motor();

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