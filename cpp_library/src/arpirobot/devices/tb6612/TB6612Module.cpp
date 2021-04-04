
#include <arpirobot/devices/tb6612/TB6612Module.hpp>
#include <arpirobot/devices/tb6612/TB6612Motor.hpp>

using namespace arpirobot;


TB6612Module::TB6612Module(int aIn1Pin, int aIn2Pin, int pwmaPin, int bIn1Pin, int bIn2Pin, int pwmbPin) : 
        motorA(aIn1Pin, aIn2Pin, pwmaPin), motorB(bIn1Pin, bIn2Pin, pwmbPin){
    
}

TB6612Motor &TB6612Module::getMotorA(){
    return motorA;
}

TB6612Motor &TB6612Module::getMotorB(){
    return motorB;
}
