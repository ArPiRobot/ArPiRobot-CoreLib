/*
 * Copyright 2021 Marcus Behel
 *
 * This file is part of ArPiRobot-CoreLib.
 * 
 * ArPiRobot-CoreLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ArPiRobot-CoreLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with ArPiRobot-CoreLib.  If not, see <https://www.gnu.org/licenses/>. 
 */


#include <arpirobot/core/drive/TankDriveHelper.hpp>
#include <arpirobot/core/log/Logger.hpp>

#include <cmath>

using namespace arpirobot;

TankDriveHelper::TankDriveHelper(MotorController &leftMotor, MotorController &rightMotor) : 
        leftMotors({std::shared_ptr<MotorController>(std::shared_ptr<MotorController>{}, &leftMotor)}), 
        rightMotors({std::shared_ptr<MotorController>(std::shared_ptr<MotorController>{}, &rightMotor)}){

}

TankDriveHelper::TankDriveHelper(std::shared_ptr<MotorController> leftMotor, std::shared_ptr<MotorController> rightMotor) : 
        leftMotors({leftMotor}), rightMotors({rightMotor}){

}

TankDriveHelper::TankDriveHelper(std::vector<std::reference_wrapper<MotorController>> leftMotors, std::vector<std::reference_wrapper<MotorController>> rightMotors){
    for(auto &motor : leftMotors){
        this->leftMotors.push_back(std::shared_ptr<MotorController>(std::shared_ptr<MotorController>{}, &motor.get()));
    }
    for(auto &motor : rightMotors){
        this->rightMotors.push_back(std::shared_ptr<MotorController>(std::shared_ptr<MotorController>{}, &motor.get()));
    }
}

TankDriveHelper::TankDriveHelper(std::vector<std::shared_ptr<MotorController>> leftMotors, std::vector<std::shared_ptr<MotorController>> rightMotors) : 
        leftMotors(leftMotors), rightMotors(rightMotors) {
    
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