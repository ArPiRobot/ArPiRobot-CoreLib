#include <arpirobot/devices/gamepad.hpp>
#include <arpirobot/core/robot.hpp>
#include <arpirobot/core/log.hpp>

#include <thread>
#include <chrono>
#include <string>

using namespace arpirobot;


class Robot : public BaseRobot{
public:

    Gamepad gp0{0};

    Robot() : BaseRobot(RobotProfile()){

    }

    void robotStarted(){
        
    }
    void robotEnabled(){
        
    }
    void robotDisabled(){
        
    }
    void enabledPeriodic(){
        
    }
    void disabledPeriodic(){

    }
    void periodic(){
        feedWatchdog();
        Logger::logDebug(std::to_string(gp0.getDpad(0)));
    }
};


int main(){
    Robot r;
    r.start();
    return 0;
}