#include <arpirobot/devices/gamepad.hpp>
#include <arpirobot/core/robot.hpp>
#include <arpirobot/core/log.hpp>
#include <arpirobot/devices/adafruitmotorhat.hpp>
#include <arpirobot/core/drive.hpp>
#include <arpirobot/core/action.hpp>

#include <thread>
#include <chrono>
#include <string>

using namespace arpirobot;


class MyAction : public Action{
public:
    void begin(){
        Logger::logDebug("MyAction starting");
    }

    void process(){
        Logger::logDebug("MyAction running.");
    }

    void finish(bool interrupted){
        Logger::logDebug("MyAction stopping");
    }

    bool shouldContinue(){
        return false;
    }
};


class Robot : public BaseRobot{
public:

    MyAction testAction;

    AdafruitMotorHatMotor flmotor {3};
    AdafruitMotorHatMotor rlmotor {4};
    AdafruitMotorHatMotor frmotor {2};
    AdafruitMotorHatMotor rrmotor {1};

    ArcadeDriveHelper driveHelper {{&flmotor, &rlmotor}, {&frmotor, &rrmotor}};

    Gamepad gp0 {0};

    Robot() : BaseRobot(){

    }

    void robotStarted(){

        ActionManager::startAction(&testAction);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        ActionManager::stopAction(&testAction);

        flmotor.setInverted(true);
        frmotor.setInverted(true);

        // Don't worry about mem leak here. Will need these until program exits.
        gp0.setAxisTransform(1, new CubicAxisTransform(0, 0.5));
        gp0.setAxisTransform(2, new SquareRootAxisTransform());
    }
    void robotEnabled(){
        
    }
    void robotDisabled(){
        
    }
    void enabledPeriodic(){
        double speed = gp0.getAxis(1, 0.1) * -1;
        double rotation = gp0.getAxis(2, 0.1);
        driveHelper.update(speed, rotation);
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