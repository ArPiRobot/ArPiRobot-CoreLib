package robot;

import arpirobot.core.log.Logger;
import arpirobot.core.network.NetworkTable;
import arpirobot.core.robot.BaseRobot;

public class Robot extends BaseRobot {

    @Override
    protected void robotStarted() {
        NetworkTable.set("TEST", "abcdefghijklmnop");
    }

    @Override
    protected void robotEnabled() {
        
    }

    @Override
    protected void robotDisabled() {
        
    }

    @Override
    protected void enabledPeriodic() {
        
    }

    @Override
    protected void disabledPeriodic() {
        
    }

    @Override
    protected void periodic() {
        
    }
    
}
