/*
 * Copyright 2022 Marcus Behel
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

#include <arpirobot/core/control/PID.hpp>
#include <algorithm>

using namespace arpirobot;


PID::PID(double kp, double ki, double kd, double kf, double min, double max) : 
        kp(kp), ki(ki), kd(kd), kf(kf), min(min), max(max) {
    
}

double PID::getKp() {
    return kp;
}

void PID::setKp(double kp) {
    this->kp = kp;
}

double PID::getKi() {
    return ki;
}

void PID::setKi(double ki) {
    this->ki = ki;
}

double PID::getKd() {
    return kd;
}

void PID::setKd(double kd) {
    this->kd = kd;
}

double PID::getKf() {
    return kf;
}

void PID::setKf(double kf) {
    this->kf = kf;
}

double PID::getMin() {
    return min;
}

void PID::setMin(double min) {
    this->min = min;
}

double PID::getMax() {
    return max;
}

void PID::setMax(double max) {
    this->max = max;
}

double PID::getSetpoint() {
    return setpoint;
}

void PID::setSetpoint(double setpoint) {
    this->setpoint = setpoint;
}

void PID::reset() {
    integral = 0;
    lastError = 0;
}

double PID::getOutput(double currentPv) {
    // Start with feedforward gain
    double output = kf * setpoint;

    // Calculate current error (used in P, I, and D calculations)
    double currError = setpoint - currentPv;

    // Proportional gain
    output += kp * currError;

    // Integral gain
    integral += currError;
    output += ki * integral;

    // Derivative gain
    output += kd * (currError - lastError);
    lastError = currError;

    // Limit output to within configured range
    return std::max(min, std::min(output, max));
}
