#include <arpirobot/devices/adafruitmotorhat.hpp>
#include <arpirobot/core/robot.hpp>
#include <arpirobot/devices/adafruitmotorhat.hpp>
#include <arpirobot/core/log.hpp>

#include <thread>
#include <chrono>
#include <string>

using namespace arpirobot;
using MotorCommand = arpirobot::AdafruitMotorHat::MotorCommand;

class Robot : public BaseRobot{
public:

    AdafruitMotorHatMotor motor{1, 0x6F, false};

    Robot() : BaseRobot(RobotProfile()){

    }

    void robotStarted(){
        
    }
    void robotEnabled(){
        motor.setSpeed(0.5);
    }
    void robotDisabled(){
        
    }
    void enabledPeriodic(){
        
    }
    void disabledPeriodic(){

    }
    void periodic(){
        feedWatchdog();
    }
};


int main(){
    Robot r;
    r.start();
    return 0;
}