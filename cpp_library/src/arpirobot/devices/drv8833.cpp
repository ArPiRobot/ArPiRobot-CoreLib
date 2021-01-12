#include <arpirobot/devices/drv8833.hpp>
#include <arpirobot/core/robot.hpp>
#include <arpirobot/core/log.hpp>

#include <pigpio.h>

using namespace arpirobot;

DRV8833Motor::DRV8833Motor(int in1Pin, int in2Pin, int slpPin) : in1(in1Pin), in2(in2Pin), slp(slpPin){
    // Always call this at the end of the device's constructor
    BaseRobot::beginWhenReady(this);
}

DRV8833Motor::~DRV8833Motor(){
    gpioPWM(in1, 0);
    gpioPWM(in2, 0);
    gpioWrite(slp, 0);
}

std::string DRV8833Motor::getDeviceName(){
    return "DRV8833Motor(" + std::to_string(in1) + ", " + std::to_string(in2) + ", " + std::to_string(slp) + ")";
}

void DRV8833Motor::begin(){
    gpioPWM(in1, 0);
    gpioPWM(in2, 0);
    gpioWrite(slp, 1);
}

void DRV8833Motor::run(){
    if(speed == 0){
        if(brakeMode){
            gpioPWM(in1, 255);
            gpioPWM(in2, 255);
        }else{
            gpioPWM(in1, 0);
            gpioPWM(in2, 0);
        }
    }else{
        int spd = (int)(std::abs(speed) * 255);
        if(speed > 0){
            gpioPWM(in1, spd);
            gpioPWM(in2, 0);
        }else{
            gpioPWM(in1, 0);
            gpioPWM(in2, spd);
        }
    }
}

DRV8833Module::DRV8833Module(int aIn1Pin, int aIn2Pin, int bIn1Pin, int bIn2Pin, int slpPin) : 
        motorA(aIn1Pin, aIn2Pin, slpPin), motorB(bIn1Pin, bIn2Pin, slpPin){
    
}

DRV8833Motor &DRV8833Module::getMotorA(){
    return motorA;
}

DRV8833Motor &DRV8833Module::getMotorB(){
    return motorB;
}
