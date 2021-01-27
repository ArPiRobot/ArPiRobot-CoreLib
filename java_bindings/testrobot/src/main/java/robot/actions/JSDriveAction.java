package robot.actions;

import arpirobot.core.action.Action;
import arpirobot.core.device.BaseDevice;
import robot.Main;

public class JSDriveAction extends Action {
    @Override
    protected void begin() {
        lockDevices(new BaseDevice[]{
            Main.robot.flmotor,
            Main.robot.frmotor,
            Main.robot.rlmotor,
            Main.robot.rrmotor
        });
    }

    @Override
    protected void process() {
        double speed = Main.robot.gp0.getAxis(1, 0.1);
        double rotation = Main.robot.gp0.getAxis(2, 0.1);
        Main.robot.driveHelper.update(speed, rotation);
    }

    @Override
    protected void finish(boolean wasInterrupted) {

    }

    @Override
    protected boolean shouldContinue() {
        return true;
    }
}
