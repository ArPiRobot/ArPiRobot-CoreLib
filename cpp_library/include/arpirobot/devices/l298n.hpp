#pragma once

#include <string>
#include <arpirobot/core/device.hpp>

namespace arpirobot{
    class L298NMotor : public MotorController{
    public:
        L298NMotor(int in1Pin, int in2Pin, int pwmPin);
        ~L298NMotor();

        std::string getDeviceName();

    protected:
        void begin();
        
        void run();

    private:
        int in1, in2, pwm;
    };

    class L298NModule{
    public:
        L298NModule(int in1Pin, int in2Pin, int enaPin, int in3Pin, int in4Pin, int enbPin);

        L298NMotor &getMotorA();
        L298NMotor &getMotorB();

    private:
        L298NMotor motorA, motorB;
    };
}