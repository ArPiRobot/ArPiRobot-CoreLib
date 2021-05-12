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

#include <arpirobot/devices/adafruitmotorhat/AdafruitMotorHatMotor.hpp>
#include <arpirobot/core/robot/BaseRobot.hpp>
#include <arpirobot/core/log/Logger.hpp>

#include <arpirobot/core/io/Io.hpp>

#include <thread>
#include <chrono>
#include <cmath>

using namespace arpirobot;
using namespace arpirobot::internal;
using MotorCommand = arpirobot::internal::AdafruitMotorHat::MotorCommand;

////////////////////////////////////////////////////////////////////////////////
/// AdafruitMotorHat::LowLevelDCMotor
////////////////////////////////////////////////////////////////////////////////

AdafruitMotorHat::LowLevelDCMotor::LowLevelDCMotor(AdafruitMotorHat *hat, int num) : hat(hat){
    switch(num){
    case 0:
        pwm = 8;
        in2 = 10;
        in1 = 9;
        break;
    case 1:
        pwm = 13;
        in2 = 12;
        in1 = 11;
        break;
    case 2:
        pwm = 2;
        in2 = 4;
        in1 = 3;
        break;
    case 3:
        pwm = 7;
        in2 = 6;
        in1 = 5;
        break;
    default:
        throw std::runtime_error("Motor number for adafruit motor hat must be 0, 1, 2, or 3.");
    }
}

void AdafruitMotorHat::LowLevelDCMotor::run(MotorCommand cmd){
    std::lock_guard<std::mutex> l(lock);
    if(!canRun)
        return;
    switch(cmd){
    case MotorCommand::FORWARD:
        hat->setPin(in2, false);
        hat->setPin(in1, true);
        break;
    case MotorCommand::BACKWARD:
        hat->setPin(in2, true);
        hat->setPin(in1, false);
        break;
    case MotorCommand::BRAKE:
        hat->setPin(in2, true);
        hat->setPin(in1, true);
        break;
    case MotorCommand::RELEASE:
        hat->setPin(in2, false);
        hat->setPin(in1, false);
        break;
    }
}

void AdafruitMotorHat::LowLevelDCMotor::setSpeed(double speed){
    // Limit speed (magnitude) between 0 and 1 (inclusive)
    if(speed < 0)
        speed = 0;
    if(speed > 1)
        speed = 1;
    
    // Don't worry about thread safety here. It doesn't really matter if speed pin is set after kill()
    int i = (int)(speed*4095);
    if(canRun)
        hat->setPWM(pwm, 0, i);
}

void AdafruitMotorHat::LowLevelDCMotor::kill(){
    std::lock_guard<std::mutex> l(lock);
    canRun = false;
    hat->setPin(in2, false);
    hat->setPin(in1, false);
}


////////////////////////////////////////////////////////////////////////////////
/// AdafruitMotorHat
////////////////////////////////////////////////////////////////////////////////

AdafruitMotorHat::AdafruitMotorHat(uint8_t address, uint8_t bus) : IoDevice(std::bind(&AdafruitMotorHat::close, this)) {
    handle = Io::i2cOpen(bus, address);
    if(handle < 0){
        throw std::runtime_error("Unable to open I2C device for adafruit motor hat.");
    }

    // Make sure there is a device at that address
    if(Io::i2cReadByte(handle) < 0){
        throw std::runtime_error("Unable to open I2C device for adafruit motor hat.");
    }

    motors[0] = std::make_shared<LowLevelDCMotor>(this, 0);
    motors[1] = std::make_shared<LowLevelDCMotor>(this, 1);
    motors[2] = std::make_shared<LowLevelDCMotor>(this, 2);
    motors[3] = std::make_shared<LowLevelDCMotor>(this, 3);

    startup();
    setPWMFreq(1600);
}

AdafruitMotorHat::~AdafruitMotorHat(){
    close();
}

std::shared_ptr<AdafruitMotorHat::LowLevelDCMotor> AdafruitMotorHat::getMotor(int index){
    return motors[index];
}

void AdafruitMotorHat::startup(){
    setAllPWM(0, 0);
    Io::i2cWriteReg8(handle, __MODE2, __OUTDRV);
    Io::i2cWriteReg8(handle, __MODE1, __ALLCALL);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    uint8_t mode1 = Io::i2cReadReg8(handle, __MODE1);
    mode1 = mode1 & ~__SLEEP;
    Io::i2cWriteReg8(handle, __MODE1, mode1);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
}

void AdafruitMotorHat::close(){
    motors[0]->run(MotorCommand::RELEASE);
    motors[1]->run(MotorCommand::RELEASE);
    motors[2]->run(MotorCommand::RELEASE);
    motors[3]->run(MotorCommand::RELEASE);
    Io::i2cClose(handle);
}

void AdafruitMotorHat::setPin(uint8_t pin, bool isHigh){
    if(isHigh){
        setPWM(pin, 4096, 0);
    }else{
        setPWM(pin, 0, 4096);
    }
}

void AdafruitMotorHat::setPWMFreq(int freq){
    float prescaleval = 25000000.0f; // 25Mhz
    prescaleval /= 4096.0f;           // 12-bit
    prescaleval /= (float)freq;
    prescaleval -= 1.0;
    double prescale = std::floor(prescaleval + 0.5);
    int oldmode = Io::i2cReadReg8(handle, __MODE1);
    int newMode = (oldmode & 0x7F) | 0x10;
    Io::i2cWriteReg8(handle, __MODE1, newMode);
    Io::i2cWriteReg8(handle, __PRESCALE, (int)std::floor(prescale));
    Io::i2cWriteReg8(handle, __MODE1, oldmode);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    Io::i2cWriteReg8(handle, __MODE1, oldmode | 0x80);
}

void AdafruitMotorHat::setPWM(uint8_t channel, int on, int off){
    Io::i2cWriteReg8(handle, __LED0_ON_L+4*channel, on & 0xFF);
    Io::i2cWriteReg8(handle, __LED0_ON_H+4*channel, on >> 8);
    Io::i2cWriteReg8(handle, __LED0_OFF_L+4*channel, off & 0xFF);
    Io::i2cWriteReg8(handle, __LED0_OFF_H+4*channel, off >> 8);
}

void AdafruitMotorHat::setAllPWM(int on, int off){
    Io::i2cWriteReg8(handle, __ALL_LED_ON_L, on & 0xFF);
    Io::i2cWriteReg8(handle, __ALL_LED_ON_H, on >> 8);
    Io::i2cWriteReg8(handle, __ALL_LED_OFF_L, off & 0xFF);
    Io::i2cWriteReg8(handle, __ALL_LED_OFF_H, off >> 8);
}


////////////////////////////////////////////////////////////////////////////////
/// AdafruitMotorHatMotor
////////////////////////////////////////////////////////////////////////////////
const int AdafruitMotorHatMotor::ADAFRUIT_ADDR = 0x60;
const int AdafruitMotorHatMotor::GEEKWORM_ADDR = 0x6F;
const int AdafruitMotorHatMotor::DETECT_ADDR = -1;

int AdafruitMotorHatMotor::detectedAddress = -1;
std::unordered_map<int, std::shared_ptr<AdafruitMotorHat>> AdafruitMotorHatMotor::hatMap;


AdafruitMotorHatMotor::AdafruitMotorHatMotor(int motorNum, int address, bool remapNumbers) : 
        motorNum(motorNum), hatAddress(address), remapNumbers(remapNumbers){

    // Always call this at the end of the device's constructor
    BaseRobot::beginWhenReady(this);
}

void AdafruitMotorHatMotor::begin(){
    if(motorNum > 4 || motorNum < 1){
        std::string msg = "AdafruitMotorHatMotor number must be 1-4.";
        Logger::logError(msg);
        throw std::runtime_error(msg);
    }

    // Handle auto detect hat address (will check adafruit first then fallback on geekworm address)
    // If both fail will log exception
    if(hatAddress == DETECT_ADDR){
        if(detectedAddress != -1){
            hatAddress = detectedAddress;
        }else{
            doDetectAddress();
            if(detectedAddress == -1){
                std::string message = "Unable to detect motor hat address.";
                Logger::logErrorFrom(getDeviceName(), message);
                throw std::runtime_error(message);
            }
            hatAddress = detectedAddress;
        }
    }

    std::shared_ptr<AdafruitMotorHat> hat;
    if(hatMap.find(hatAddress) != hatMap.end()){
        hat = hatMap[hatAddress];
    }else{
        hat = std::make_shared<AdafruitMotorHat>(hatAddress);
        hatMap[hatAddress] = hat;
    }

    // Remap motor number if necessary
    int internalMotorNumber = motorNum;
    if(remapNumbers){
        internalMotorNumber = remapMotorNumber(hatAddress, motorNum);
    }

    motor = hat->getMotor(internalMotorNumber - 1);
}

int AdafruitMotorHatMotor::remapMotorNumber(int hatAddress, int motorNum){
    if(hatAddress == GEEKWORM_ADDR){
        switch(motorNum){
        case 1:
            return 3;
        case 2:
            return 4;
        case 3:
            return 1;
        case 4:
            return 2;
        }
    }
    return motorNum;
}

void AdafruitMotorHatMotor::doDetectAddress(){
    AdafruitMotorHat *testHat = nullptr;
    try{
        testHat = new AdafruitMotorHat(ADAFRUIT_ADDR);
        detectedAddress = ADAFRUIT_ADDR;
    }catch(const std::runtime_error &e){   
        detectedAddress = GEEKWORM_ADDR;
    }
    if(testHat != nullptr){
        delete testHat;
    }
}

void AdafruitMotorHatMotor::run(){
    if(speed < 0){
        motor->setSpeed(speed * -1);
        motor->run(MotorCommand::BACKWARD);
    }else if (speed > 0){
        motor->setSpeed(speed);
        motor->run(MotorCommand::FORWARD);
    }else if(brakeMode){
        motor->setSpeed(0);
        motor->run(MotorCommand::BRAKE);
    }else{
        motor->setSpeed(0);
        motor->run(MotorCommand::RELEASE);
    }
}

std::string AdafruitMotorHatMotor::getDeviceName(){
    return "AdafruitMotorHatMotor(" + std::to_string(motorNum) + ", " + std::to_string(hatAddress) + ")";
}
