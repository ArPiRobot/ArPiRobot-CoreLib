
from arpirobot.core.audio import AudioManager
from arpirobot.core.log import Logger
from arpirobot.core.action import Action
from arpirobot.core.control import PID
from arpirobot.core.network import NetworkTable

import main 

class TestAction(Action):
    def __init__(self):
        super().__init__()
        self.pid = PID()

    def begin(self):
        self.pid.reset()
        self.pid.set_max(1.0)
        self.pid.set_min(-1.0)
        self.pid.set_kp(1.0)
        self.pid.set_ki(0.0)
        self.pid.set_kd(0.0)
        self.pid.set_setpoint(main.robot.imu.get_gyro_z() + 90.0)

    def process(self):
        if NetworkTable.changed("P"):
            try:
                self.pid.set_kp(float(NetworkTable.get("P")))
            except ValueError:
                pass
        if NetworkTable.changed("I"):
            try:
                self.pid.set_ki(float(NetworkTable.get("I")))
            except ValueError:
                pass
        if NetworkTable.changed("D"):
            try:
                self.pid.set_kd(float(NetworkTable.get("D")))
            except ValueError:
                pass
        main.robot.drive_helper.update_rotation(-self.pid.get_output(main.robot.imu.get_gyro_z()))
    
    def finish(self, was_interrupted: bool):
        main.robot.drive_helper.update_rotation(0)
    
    def should_continue(self) -> bool:
        return True