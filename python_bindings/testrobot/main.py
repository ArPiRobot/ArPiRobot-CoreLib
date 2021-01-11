

class Main:
    robot: 'robot_mod.Robot' = None


import robot as robot_mod


if __name__ == "__main__":
    from main import Main # No this is not redundant!
    Main.robot = robot_mod.Robot()
    Main.robot.start()