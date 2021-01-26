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
        return Bridge.arpirobot.BaseAxisTransform_applyTransform(ptr, rawAxisValue);
    }
}
