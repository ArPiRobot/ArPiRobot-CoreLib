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

package arpirobot.arduino.iface;

import arpirobot.Bridge;

/**
 * Arduino interface implementation using UART to communicate with the arduino.
 */
public class ArduinoUartInterface extends BaseArduinoInterface {
    
    /**
     * @param port The UART port for the arduino (/dev/tty...)
     * @param baud The baud rate for UART communication
     */
    public ArduinoUartInterface(String port, int baud){
        ptr = Bridge.arpirobot.ArduinoUartInterface_create(port, baud);
    }

    @Override
    protected void doDestroy() {
        Bridge.arpirobot.ArduinoUartInterface_destroy(ptr);
    }
}
