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

package arpirobot.core.drive;

import arpirobot.Bridge;
import arpirobot.DestroyableObject;

/**
 * Base class for axis transforms. Axis transforms take an analog axis value (-1.0 to 1.0)
 * and apply a fixed calculation to modify the curve of the axis.
 * 
 * Axis transforms are most useful with drive control.
 */
public abstract class BaseAxisTransform extends DestroyableObject {
    /**
     * Axis transforms should override this function to implement specific calculations.
     */
    public double applyTransform(double rawAxisValue){
        verifyNotDestroyed();
        return Bridge.arpirobot.BaseAxisTransform_applyTransform(ptr, rawAxisValue);
    }
}
