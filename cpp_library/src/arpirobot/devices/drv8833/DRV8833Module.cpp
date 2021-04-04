
#include <arpirobot/devices/drv8833/DRV8833Module.hpp>
#include <arpirobot/devices/drv8833/DRV8833Motor.hpp>


using namespace arpirobot;

DRV8833Module::DRV8833Module(int aIn1Pin, int aIn2Pin, int bIn1Pin, int bIn2Pin, int slpPin) : 
        motorA(aIn1Pin, aIn2Pin, slpPin), motorB(bIn1Pin, bIn2Pin, slpPin){
    
}

DRV8833Motor &DRV8833Module::getMotorA(){
    return motorA;
}

DRV8833Motor &DRV8833Module::getMotorB(){
    return motorB;
}