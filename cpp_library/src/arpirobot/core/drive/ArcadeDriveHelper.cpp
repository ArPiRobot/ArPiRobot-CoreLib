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

#include <arpirobot/core/drive/ArcadeDriveHelper.hpp>
#include <arpirobot/core/log/Logger.hpp>

#include <cmath>

using namespace arpirobot;


ArcadeDriveHelper::ArcadeDriveHelper(MotorController *leftMotor, MotorController *rightMotor){
    this->leftMotors.push_back(leftMotor);
    this->rightMotors.push_back(rightMotor);
}

ArcadeDriveHelper::ArcadeDriveHelper(std::vector<MotorController*> leftMotors, std::vector<MotorController*> rightMotors){
    for(auto &motor : leftMotors){
        this->leftMotors.push_back(motor);
    }
    for(auto &motor : rightMotors){
        this->rightMotors.push_back(motor);
    }
}

void ArcadeDriveHelper::updateSpeed(double newSpeed){
    speed = newSpeed;
    doUpdate();
}

void ArcadeDriveHelper::updateRotation(double newRotation){
    rotation = newRotation;
    doUpdate();
}

void ArcadeDriveHelper::update(double newSpeed, double newRotation){
    speed = newSpeed;
    rotation = newRotation;
    doUpdate();
}

std::array<double, 2> ArcadeDriveHelper::calculateSpeeds(double speed, double rotation){
    double left = 0, right = 0;

    // Don't allow speed or rotation to be negative zero
    speed += 0.0;
    rotation += 0.0;

    double maxInput = std::copysign(std::max(std::abs(speed), std::abs(rotation)), speed);

    if((rotation >= 0 && speed >= 0) || (rotation < 0 && speed < 0)){
        left = maxInput;
        right = speed - rotation;
    }else{
        left = speed + rotation;
        right = maxInput;
    }

    return {left, right};
}

void ArcadeDriveHelper::doUpdate(){
    // Restrict range of speed and rotation
    if(speed > 1)
        speed = 1;
    if(speed < -1)
        speed = -1;
    if(rotation > 1)
        rotation = 1;
    if(rotation < -1)
        rotation = -1;
    
    auto speeds = calculateSpeeds(speed, rotation);

    for(auto motor : leftMotors){
        motor->setSpeed(speeds[0]);
    }
    for(auto motor : rightMotors){
        motor->setSpeed(speeds[1]);
    }
}