package arpirobot;

import javax.security.auth.callback.Callback;

import com.sun.jna.Library;
import com.sun.jna.Native;

public class Bridge {

    public interface vptr_t extends Callback{
        void invoke();
    }

    public interface CLibrary extends Library {
        public CLibrary instance = (CLibrary)Native.load("arpirobot-core", CLibrary.class);

        public long BaseRobot_create(vptr_t robotStarted, vptr_t robotEnabled, vptr_t robotDisabled, vptr_t enabledPeriodic, 
                vptr_t disabledPeriodic, vptr_t periodic, int mainSchedulerThreads, int periodicFunctionRate, 
                int maxGamepadDataAge, int actionFunctionPeriod); 
    }
}
