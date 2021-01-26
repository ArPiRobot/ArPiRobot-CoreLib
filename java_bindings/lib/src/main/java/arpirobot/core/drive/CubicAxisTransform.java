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
