#include <arpirobot/devices/l298n.hpp>
#include <arpirobot/core/robot.hpp>
#include <arpirobot/core/log.hpp>

#include <pigpio.h>

using namespace arpirobot;

L298NMotor::L298NMotor(int in1Pin, int in2Pin, int pwmPin) : in1(in1Pin), in2(in2Pin), pwm(pwmPin){
    // Always call this at the end of the device's constructor
    BaseRobot::beginWhenReady(this);
}

L298NMotor::~L298NMotor(){
    gpioWrite(in1, 0);
    gpioWrite(in2, 0);
    gpioPWM(pwm, 0);
}

std::string L298NMotor::getDeviceName(){
    return "L298NMotor(" + std::to_string(in1) + ", " + std::to_string(in2) + ", " + std::to_string(pwm) + ")";
}

void L298NMotor::begin(){
    gpioSetMode(in1, PI_OUTPUT);
    gpioSetMode(in2, PI_OUTPUT);
    gpioPWM(pwm, 0);
}

void L298NMotor::run(){
    if(speed == 0){
        gpioWrite(in1, 0);
        gpioWrite(in2, 0);
        if(brakeMode){
            gpioPWM(pwm, 255);
        }else{
            gpioPWM(pwm, 0);
        }
    }else{
        if(speed > 0){
            gpioWrite(in1, 1);
            gpioWrite(in2, 0);
        }else{
            gpioWrite(in1, 0);
            gpioWrite(in2, 1);
        }
        gpioPWM(pwm, (int)(std::abs(speed) * 255));
    }
}

L298NModule::L298NModule(int in1Pin, int in2Pin, int enaPin, int in3Pin, int in4Pin, int enbPin) : 
        motorA(in1Pin, in2Pin, enaPin), motorB(in3Pin, in4Pin, enbPin){
    
}

L298NMotor &L298NModule::getMotorA(){
    return motorA;
}

L298NMotor &L298NModule::getMotorB(){
    return motorB;
}
