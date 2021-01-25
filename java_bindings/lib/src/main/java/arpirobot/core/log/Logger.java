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
