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

package arpirobot.core.log;

import arpirobot.Bridge;

public class Logger {

    /**
     * Log a debug message
     * @param message The message to log
     */
    public static void logDebug(String message){
        Bridge.arpirobot.Logger_logDebug(message);
    }

    /**
     * Log an info message
     * @param message The message to log
     */
    public static void logInfo(String message){
        Bridge.arpirobot.Logger_logInfo(message);
    }

    /**
     * Log a warning message
     * @param message The message to log
     */
    public static void logWarning(String message){
        Bridge.arpirobot.Logger_logWarning(message);
    }

    /**
     * Log an error message
     * @param message The message to log
     */
    public static void logError(String message){
        Bridge.arpirobot.Logger_logError(message);
    }

    /**
     * Log a debug message from a specific source
     * @param source The source of a message (often a device name)
     * @param message The messsage to log
     */
    public static void logDebugFrom(String source, String message){
        Bridge.arpirobot.Logger_logDebugFrom(source, message);
    }

    /**
     * Log a info message from a specific source
     * @param source The source of a message (often a device name)
     * @param message The messsage to log
     */
    public static void logInfoFrom(String source, String message){
        Bridge.arpirobot.Logger_logInfoFrom(source, message);
    }

    /**
     * Log a warning message from a specific source
     * @param source The source of a message (often a device name)
     * @param message The messsage to log
     */
    public static void logWarningFrom(String source, String message){
        Bridge.arpirobot.Logger_logWarningFrom(source, message);
    }

    /**
     * Log a error message from a specific source
     * @param source The source of a message (often a device name)
     * @param message The messsage to log
     */
    public static void logErrorFrom(String source, String message){
        Bridge.arpirobot.Logger_logErrorFrom(source, message);
    }

    /**
     * Add an empty line to the log
     */
    public static void logNewline(){
        Bridge.arpirobot.Logger_logNewline();
    }
}
