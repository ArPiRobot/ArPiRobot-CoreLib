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
