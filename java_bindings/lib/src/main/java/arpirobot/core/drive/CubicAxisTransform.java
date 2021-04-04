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

/** 
 * Transform that applies a cubic function to the magnitude of the input.
 * The cubic function is fit to the data set (0, minPower), (0.5, midPower), (1, 1)
 * This gives a wide range of inputs that will yield a value near midPower
 * Sign of the output will match the sign of the input
 */
public class CubicAxisTransform extends BaseAxisTransform {

    /**
     * @param minPower The minimum output of this cubic function (leave as zero if unsure)
     * @param midPower The "middle" power where the "flat" part of the function should be positioned
     */
    public CubicAxisTransform(double minPower, double midPower){
        ptr = Bridge.arpirobot.CubicAxisTransform_create(minPower, midPower);
    }

    @Override
    protected void doDestroy() {
        Bridge.arpirobot.CubicAxisTransform_destroy(ptr);
    }
}
