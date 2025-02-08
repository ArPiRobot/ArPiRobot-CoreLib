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

#pragma once

#include <mutex>
#include <string>

namespace arpirobot {

    /**
     * \class Logger Logger.hpp arpirobot/core/log/Logger.hpp
     * 
     * Helper class with static methods for logging messages.
     * Log messages will be printed to stdout and sent to a connected DS.
     */
    class Logger{
    public:

        /**
         * Log a debug message
         * @param message The message to log
         */
        static void logDebug(std::string message);
        
        /**
         * Log an info message
         * @param message The message to log
         */
        static void logInfo(std::string message);

        /**
         * Log a warning message
         * @param message The message to log
         */
        static void logWarning(std::string message);

        /**
         * Log an error message
         * @param message The message to log
         */
        static void logError(std::string message);

        /**
         * Log a debug message from a specific source
         * @param source The source of a message (often a device name)
         * @param message The messsage to log
         */
        static void logDebugFrom(std::string source, std::string message);

        /**
         * Log a info message from a specific source
         * @param source The source of a message (often a device name)
         * @param message The messsage to log
         */
        static void logInfoFrom(std::string source, std::string message);

        /**
         * Log a warning message from a specific source
         * @param source The source of a message (often a device name)
         * @param message The messsage to log
         */
        static void logWarningFrom(std::string source, std::string message);

        /**
         * Log a error message from a specific source
         * @param source The source of a message (often a device name)
         * @param message The messsage to log
         */
        static void logErrorFrom(std::string source, std::string message);

        /**
         * Add an empty line to the log
         */
        static void logNewline();

    private:
        static void log(std::string message);

        static std::mutex logMutex;
    };
}