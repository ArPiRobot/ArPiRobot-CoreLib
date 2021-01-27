package robot;

import arpirobot.core.action.ActionManager;
import arpirobot.core.device.MotorController;
import arpirobot.core.drive.ArcadeDriveHelper;
import arpirobot.core.drive.CubicAxisTransform;
import arpirobot.core.drive.SquareRootAxisTransform;
import arpirobot.core.log.Logger;
import arpirobot.core.robot.BaseRobot;
import arpirobot.devices.adafruitmotorhat.AdafruitMotorHatMotor;
import arpirobot.devices.gamepad.Gamepad;
import robot.actions.JSDriveAction;

public class Robot extends BaseRobot {

    public AdafruitMotorHatMotor flmotor = new AdafruitMotorHatMotor(3);
    public AdafruitMotorHatMotor rlmotor = new AdafruitMotorHatMotor(4);
    public AdafruitMotorHatMotor frmotor = new AdafruitMotorHatMotor(2);
    public AdafruitMotorHatMotor rrmotor = new AdafruitMotorHatMotor(1);

    public ArcadeDriveHelper driveHelper = new ArcadeDriveHelper(
        new MotorController[]{flmotor, rlmotor},
        new MotorController[]{frmotor, rrmotor}
    );

    public Gamepad gp0 = new Gamepad(0);


    @Override
    protected void robotStarted() {

        flmotor.setInverted(true);
        frmotor.setInverted(true);

        // gp0.setAxisTransform(1, new CubicAxisTransform(0, 0.5));
        // gp0.setAxisTransform(2, new SquareRootAxisTransform());

        // ActionManager.startAction(new JSDriveAction());
    }

    @Override
    protected void robotEnabled() {
        
    }

    @Override
    protected void robotDisabled() {
        
    }

    @Override
    protected void enabledPeriodic() {
        
        Logger.logDebug("Button: " + gp0.getButton(1));

        if(true)
            return;
        double speed = Main.robot.gp0.getAxis(1, 0.1);
        double rotation = Main.robot.gp0.getAxis(2, 0.1);
        Logger.logDebug(speed + ", " + rotation);
        //Main.robot.driveHelper.update(speed, rotation);
    }

    @Override
    protected void disabledPeriodic() {
        
    }

    @Override
    protected void periodic() {
        
    }
    
}
