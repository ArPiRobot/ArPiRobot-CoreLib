#pragma once

#include <arpirobot/core/device.hpp>

namespace arpirobot{

    class DRV8833Motor : public MotorController{
    public:
        DRV8833Motor(int in1Pin, int in2Pin, int slpPin);
        ~DRV8833Motor();

        std::string getDeviceName();

    protected:
        void begin();
        
        void run();

    private:
        int in1, in2, slp;
    };

    class DRV8833Module{
    public:
        DRV8833Module(int aIn1Pin, int aIn2Pin, int bIn1Pin, int bIn2Pin, int slpPin);

        DRV8833Motor &getMotorA();
        DRV8833Motor &getMotorB();

    private:
        DRV8833Motor motorA, motorB;
    };
    
}
