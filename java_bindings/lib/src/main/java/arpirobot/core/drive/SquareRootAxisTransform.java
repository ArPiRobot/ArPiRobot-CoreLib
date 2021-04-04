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
 * Transform that applies a square root function to the magnitude of the input.
 * Sign of the output will match the sign of the input
 */
public class SquareRootAxisTransform extends BaseAxisTransform {

    public SquareRootAxisTransform(){
        ptr = Bridge.arpirobot.SquareRootAxisTransform_create();
    }

    @Override
    protected void doDestroy() {
        Bridge.arpirobot.SquareRootAxisTransform_destroy(ptr);   
    }
}
