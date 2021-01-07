#pragma once

#include <mutex>
#include <string>

namespace arpirobot {
    class Logger{
    public:
        static void logDebug(std::string message);
        
        static void logInfo(std::string message);

        static void logWarning(std::string message);

        static void logError(std::string message);

        static void logDebugFrom(std::string source, std::string message);

        static void logInfoFrom(std::string source, std::string message);

        static void logWarningFrom(std::string source, std::string message);

        static void logErrorFrom(std::string source, std::string message);

    private:
        static void log(std::string message);

        static std::mutex logMutex;
    };
}