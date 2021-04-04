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

package arpirobot.devices.adafruitmotorhat;

import arpirobot.Bridge;
import arpirobot.core.device.MotorController;

/**
 * Motor for Adafruit Motor Hat
 */
public class AdafruitMotorHatMotor extends MotorController {

    public static final int DETECT_ADDR = -1;
    public static final int ADAFRUIT_ADDR = 0x60;
    public static final int GEEKWORK_ADDR = 0x6F;

    /**
     * @param motorNum The number for the motor
     */
    public AdafruitMotorHatMotor(int motorNum){
        this(motorNum, DETECT_ADDR);
    }

    /**
     * @param motorNum The number for the motor
     * @param address The address of the hat for the motor
     */
    public AdafruitMotorHatMotor(int motorNum, int address){
        this(motorNum, address, true);
    }

    /**
     * @param motorNum The number for the motor
     * @param address The address of the hat for the motor
     * @param remapNumbers If true the motor numbers will be remapped (if needed) to match the 
     *                     physical order of the Adafruit motor hat
     */
    public AdafruitMotorHatMotor(int motorNum, int address, boolean remapNumbers){
        ptr = Bridge.arpirobot.AdafruitMotorHatMotor_create(motorNum, address, remapNumbers);
    }

    @Override
    protected void doDestroy() {
        Bridge.arpirobot.AdafruitMotorHatMotor_destroy(ptr);
    }
}
