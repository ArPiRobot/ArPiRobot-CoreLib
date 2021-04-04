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