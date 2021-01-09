#include <arpirobot/devices/gamepad.hpp>
#include <arpirobot/core/robot.hpp>
#include <arpirobot/core/log.hpp>
#include <arpirobot/devices/adafruitmotorhat.hpp>
#include <arpirobot/core/drive.hpp>

#include <thread>
#include <chrono>
#include <string>

using namespace arpirobot;


class Robot : public BaseRobot{
public:

    AdafruitMotorHatMotor flmotor {3};
    AdafruitMotorHatMotor rlmotor {4};
    AdafruitMotorHatMotor frmotor {2};
    AdafruitMotorHatMotor rrmotor {1};

    TankDriveHelper driveHelper {{&flmotor, &rlmotor}, {&frmotor, &rrmotor}};

    Gamepad gp0 {0};

    Robot() : BaseRobot(){

    }

    void robotStarted(){
        flmotor.setInverted(true);
        frmotor.setInverted(true);
    }
    void robotEnabled(){
        
    }
    void robotDisabled(){
        
    }
    void enabledPeriodic(){
        double left = gp0.getAxis(1, 0.1) * -1;
        double right = gp0.getAxis(3, 0.1) * -1;
        driveHelper.update(left, right);
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
}