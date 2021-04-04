#pragma once

#include <unordered_map>
#include <string>
#include <mutex>

namespace arpirobot{
    /**
     * \class NetworkTable network.hpp arpirobot/core/network.hpp
     * 
     * Helper class used to manage network table key/value pairs
     */
    class NetworkTable{
    public:

        /**
         * Sets a given key/value pair. If the key does not exist it will be created. Else the value will be updated.
         * @param key The key for the pair
         * @param value The value for the pair
         */
        static void set(std::string key, std::string value);

        /**
         * Get the value for a key/value pair
         * @param key The key to get the associated value with
         * @return The value associated with the given key. If the key does not exist an empty string is returned.
         */
        static std::string get(std::string key);

        /**
         * Check if a key has a value
         * @param key The key to check for a value associated with
         * @return true if a value exists for the given key, else false
         */
        static bool has(std::string key);

    private:
        static bool isInSync();
        static void startSync();
        static void sendAllValues();
        static void finishSync(std::unordered_map<std::string, std::string> dataFromDs);
        static void abortSync();

        static void setFromRobot(std::string key, std::string value);
        static void setFromDs(std::string key, std::string value);
    
        static std::unordered_map<std::string, std::string> data;
        static std::mutex lock;
        static bool inSync;

        friend class NetworkManager;
    };

}