#pragma once

#include <mutex>
#include <string>

namespace arpirobot {

    /**
     * \class Logger log.hpp arpirobot/core/log.hpp
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