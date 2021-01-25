package robot;

import arpirobot.Bridge;

public class Main {
    public static void main(String[] args){
        long ptr = Bridge.CLibrary.instance.BaseRobot_create(robotStarted, robotEnabled, robotDisabled, enabledPeriodic, disabledPeriodic, periodic, 10, 50, 100, 50);
    }

    static Bridge.vptr_t robotStarted = new Bridge.vptr_t(){
        @Override
        public void invoke(){

        }
    };
    static Bridge.vptr_t robotEnabled = new Bridge.vptr_t(){
        @Override
        public void invoke(){

        }
    };
    static Bridge.vptr_t robotDisabled = new Bridge.vptr_t(){
        @Override
        public void invoke(){

        }
    };
    static Bridge.vptr_t enabledPeriodic = new Bridge.vptr_t(){
        @Override
        public void invoke(){

        }
    };
    static Bridge.vptr_t disabledPeriodic = new Bridge.vptr_t(){
        @Override
        public void invoke(){

        }
    };
    static Bridge.vptr_t periodic = new Bridge.vptr_t(){
        @Override
        public void invoke(){

        }
    };
}
