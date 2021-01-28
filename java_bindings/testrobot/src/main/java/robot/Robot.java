package robot;

import arpirobot.core.device.MotorController;
import arpirobot.core.drive.ArcadeDriveHelper;
import arpirobot.core.robot.BaseRobot;
import arpirobot.devices.adafruitmotorhat.AdafruitMotorHatMotor;
import arpirobot.devices.gamepad.Gamepad;

public class Robot extends BaseRobot {

    public Gamepad gp0 = new Gamepad(0);

    public AdafruitMotorHatMotor flmotor = new AdafruitMotorHatMotor(3);
    public AdafruitMotorHatMotor rlmotor = new AdafruitMotorHatMotor(4);
    public AdafruitMotorHatMotor frmotor = new AdafruitMotorHatMotor(2);
    public AdafruitMotorHatMotor rrmotor = new AdafruitMotorHatMotor(1);

    public ArcadeDriveHelper driveHelper = new ArcadeDriveHelper(
        new MotorController[]{flmotor, rlmotor},
        new MotorController[]{frmotor, rrmotor}
    );

    @Override
    protected void robotStarted() {
        flmotor.setInverted(true);
        frmotor.setInverted(true);
    }

    @Override
    protected void robotEnabled() {
        
    }

    @Override
    protected void robotDisabled() {
        
    }

    @Override
    protected void enabledPeriodic() {
        driveHelper.update(0.5, 0);
    }

    @Override
    protected void disabledPeriodic() {
        
    }

    @Override
    protected void periodic() {
        feedWatchdog();
        //Logger.logDebug("" + gp0.getAxis(1, 0.1));
    }
    
}
