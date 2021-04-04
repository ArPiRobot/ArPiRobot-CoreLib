#include <arpirobot/devices/tb6612/TB6612Motor.hpp>
#include <arpirobot/devices/tb6612/TB6612Module.hpp>
#include <arpirobot/core/robot/BaseRobot.hpp>
#include <arpirobot/core/log/Logger.hpp>

#include <pigpio.h>

using namespace arpirobot;

TB6612Motor::TB6612Motor(int in1Pin, int in2Pin, int pwmPin) : in1(in1Pin), in2(in2Pin), pwm(pwmPin){
    // Always call this at the end of the device's constructor
    BaseRobot::beginWhenReady(this);
}

TB6612Motor::~TB6612Motor(){
    gpioWrite(in1, 0);
    gpioWrite(in2, 0);
    gpioPWM(pwm, 0);
}

std::string TB6612Motor::getDeviceName(){
    return "TB6612Motor(" + std::to_string(in1) + ", " + std::to_string(in2) + ", " + std::to_string(pwm) + ")";
}

void TB6612Motor::begin(){
    gpioSetMode(in1, PI_OUTPUT);
    gpioSetMode(in2, PI_OUTPUT);
    gpioPWM(pwm, 0);
}

void TB6612Motor::run(){
    if(speed == 0){
        if(brakeMode){
            gpioWrite(in1, 1);
            gpioWrite(in2, 1);
        }else{
            gpioWrite(in1, 0);
            gpioWrite(in2, 0);
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
