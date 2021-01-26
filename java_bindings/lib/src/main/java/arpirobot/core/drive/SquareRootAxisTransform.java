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
