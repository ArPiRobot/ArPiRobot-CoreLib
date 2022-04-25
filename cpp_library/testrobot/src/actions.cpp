#include <actions.hpp>
#include <main.hpp>

#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/network/NetworkTable.hpp>


void TestAction::begin(){
    pid.reset();
    pid.setMax(1.0);
    pid.setMin(-1.0);
    pid.setKp(1.0);
    pid.setKi(0.0);
    pid.setKd(0.0);
    pid.setSetpoint(Main::robot->imu.getGyroZ() + 90.0);
}

void TestAction::process(){
    if(NetworkTable::changed("P")){
        try{
            pid.setKp(std::stod(NetworkTable::get("P")));
        }catch(const std::invalid_argument &e){
            // Do not change P
        }
    }
    if(NetworkTable::changed("I")){
        try{
            pid.setKi(std::stod(NetworkTable::get("I")));
        }catch(const std::invalid_argument &e){
            // Do not change I
        }
    }
    if(NetworkTable::changed("D")){
        try{
            pid.setKd(std::stod(NetworkTable::get("D")));
        }catch(const std::invalid_argument &e){
            // Do not change D
        }
    }
    Main::robot->driveHelper.updateRotation(-pid.getOutput(Main::robot->imu.getGyroZ()));
}

void TestAction::finish(bool wasInterrupted){
    Main::robot->driveHelper.updateRotation(0);
}

bool TestAction::shouldContinue(){
    return true;
}
