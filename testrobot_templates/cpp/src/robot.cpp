#include <robot.hpp>

#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/action/ActionManager.hpp>
#include <arpirobot/core/network/NetworkTable.hpp>

#include <string>

using namespace arpirobot;


void Robot::robotStarted(){
    arduino.addDevice(&imu);
    arduino.begin();
    imu.calibrate(10);

    NetworkTable::set("P", "1.0");
    NetworkTable::set("I", "0.0");
    NetworkTable::set("D", "0.0");

    flmotor.setInverted(true);
    frmotor.setInverted(true);

    flmotor.setBrakeMode(true);
    frmotor.setBrakeMode(true);
    rrmotor.setBrakeMode(true);
    rlmotor.setBrakeMode(true);

    myTestAction.setProcessPeriodMs(20);
}

void Robot::robotEnabled(){
    ActionManager::startAction(&myTestAction);
}

void Robot::robotDisabled(){
    ActionManager::stopAction(&myTestAction);
}

void Robot::enabledPeriodic(){
    
}

void Robot::disabledPeriodic(){

}

void Robot::periodic(){
    NetworkTable::set("Gyro", std::to_string(imu.getGyroZ()));
    // Do not remove this line or some devices will be disabled.
    feedWatchdog();
}