
#include <arpirobot/core/drive/TankDriveHelper.hpp>
#include <arpirobot/core/log/Logger.hpp>

#include <cmath>

using namespace arpirobot;

TankDriveHelper::TankDriveHelper(MotorController *leftMotor, MotorController *rightMotor){
    this->leftMotors.push_back(leftMotor);
    this->rightMotors.push_back(rightMotor);
}

TankDriveHelper::TankDriveHelper(std::vector<MotorController*> leftMotors, std::vector<MotorController*> rightMotors){
    for(auto &motor : leftMotors){
        this->leftMotors.push_back(motor);
    }
    for(auto &motor : rightMotors){
        this->rightMotors.push_back(motor);
    }
}

void TankDriveHelper::updateLeftSpeed(double newLeftSpeed){
    leftSpeed = newLeftSpeed;
    doUpdate();
}

void TankDriveHelper::updateRightSpeed(double newRightSpeed){
    rightSpeed = newRightSpeed;
    doUpdate();
}

void TankDriveHelper::update(double newLeftSpeed, double newRightSpeed){
    leftSpeed = newLeftSpeed;
    rightSpeed = newRightSpeed;
    doUpdate();
}

void TankDriveHelper::doUpdate(){
    // Restrict speed ranges
    if(leftSpeed > 1)
        leftSpeed = 1;
    if(leftSpeed < -1)
        leftSpeed = -1;
    if(rightSpeed > 1)
        rightSpeed = 1;
    if(rightSpeed < -1)
        rightSpeed = -1;

    for(auto motor : leftMotors){
        motor->setSpeed(leftSpeed);
    }
    for(auto motor : rightMotors){
        motor->setSpeed(rightSpeed);
    }
}