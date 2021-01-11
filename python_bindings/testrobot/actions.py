from arpirobot.core.action import Action
import time
from main import Main


class JSDriveAction(Action):
    def begin(self):
        self.lock_devices([Main.robot.flmotor, Main.robot.frmotor, Main.robot.rlmotor, Main.robot.rrmotor])

    def process(self):
        speed = Main.robot.gp0.get_axis(1, 0.1) * -1
        rotation = Main.robot.gp0.get_axis(2, 0.1)
        Main.robot.drive_helper.update(speed, rotation)

    def finish(self, interrupted: bool):
        Main.robot.drive_helper.update(0, 0)

    def should_continue(self) -> bool:
        return True


class ButtonDriveAction(Action):
    def __init__(self):
        super().__init__()
        self.__start_time = 0
    
    def begin(self):
        self.__start_time = time.time()
        
        self.lock_devices([Main.robot.flmotor, Main.robot.frmotor, Main.robot.rlmotor, Main.robot.rrmotor])
        
        Main.robot.drive_helper.update(0.5, 0)
    
    def process(self):
        pass

    def finish(self, interrupted: bool):
        Main.robot.drive_helper.update(0, 0)

    def should_continue(self) -> bool:
        return time.time() - self.__start_time < 1000