#include <arpirobot/devices/l298n/L298NModule.hpp>
#include <arpirobot/devices/l298n/L298NMotor.hpp>

using namespace arpirobot;


L298NModule::L298NModule(int in1Pin, int in2Pin, int enaPin, int in3Pin, int in4Pin, int enbPin) : 
        motorA(in1Pin, in2Pin, enaPin), motorB(in3Pin, in4Pin, enbPin){
    
}

L298NMotor &L298NModule::getMotorA(){
    return motorA;
}

L298NMotor &L298NModule::getMotorB(){
    return motorB;
}
