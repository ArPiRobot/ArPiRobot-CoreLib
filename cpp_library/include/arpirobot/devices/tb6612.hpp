#pragma once

#include <arpirobot/core/device.hpp>

namespace arpirobot{
    class TB6612Motor : public MotorController{
    public:
        TB6612Motor(int in1Pin, int in2Pin, int pwmPin);
        ~TB6612Motor();

        std::string getDeviceName();

    protected:
        void begin();
        
        void run();

    private:
        int in1, in2, pwm;
    };

    class TB6612Module{
    public:
        TB6612Module(int aIn1Pin, int aIn2Pin, int pwmaPin, int bIn1Pin, int bIn2Pin, int pwmbPin);

        TB6612Motor &getMotorA();
        TB6612Motor &getMotorB();

    private:
        TB6612Motor motorA, motorB;
    };
}