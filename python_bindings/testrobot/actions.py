from arpirobot.core.action import Action
import time
import main
from arpirobot.core.log import Logger


class JSDriveAction(Action):
    def begin(self):
        self.lock_devices([
            main.robot.flmotor,
            main.robot.frmotor,
            main.robot.rlmotor,
            main.robot.rrmotor
        ])

    def process(self):
        speed = main.robot.gp0.get_axis(1, 0.1) * -1
        rotation = main.robot.gp0.get_axis(2, 0.1)
        main.robot.drive_helper.update(speed, rotation)

    def finish(self, interrupted: bool):
        main.robot.drive_helper.update(0, 0)

    def should_continue(self) -> bool:
        return True