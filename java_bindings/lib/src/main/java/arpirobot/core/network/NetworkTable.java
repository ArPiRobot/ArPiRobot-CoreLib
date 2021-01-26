package arpirobot.core.network;

import arpirobot.Bridge;
import com.sun.jna.Pointer;


/**
 * Helper class used to manage network table key/value pairs
 */
public class NetworkTable {
    /**
     * Sets a given key/value pair. If the key does not exist it will be created. Else the value will be updated.
     * @param key The key for the pair
     * @param value The value for the pair
     */
    public static void set(String key, String value){
        Bridge.arpirobot.NetworkTable_set(key, value);
    }

    /**
     * Get the value for a key/value pair
     * @param key The key to get the associated value with
     * @return The value associated with the given key. If the key does not exist an empty string is returned.
     */
    public static String get(String key){
        Pointer ptr = Bridge.arpirobot.NetworkTable_get(key);
        String str = ptr.getString(0);
        Bridge.arpirobot.freeString(ptr);
        return str;
    }

    /**
     * Check if a key has a value
     * @param key The key to check for a value associated with
     * @return true if a value exists for the given key, else false
     */
    public static boolean has(String key){
        return Bridge.arpirobot.NetworkTable_has(key);
    }
}
