"""
Copyright 2021 Marcus Behel

This file is part of ArPiRobot-CoreLib.

ArPiRobot-CoreLib is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ArPiRobot-CoreLib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with ArPiRobot-CoreLib.  If not, see <https://www.gnu.org/licenses/>. 
"""

import arpirobot.bridge as bridge


## Implementation of a simple PID controller with feedforward gain support.
class PID:
    ## Construct a new PID object
    #  @param kp Proportional gain
    #  @param ki Integral gain
    #  @param kd Derivative gain
    #  @param kf Feedforward gain (often not used, but useful for some systems)
    #  @param min Minimum value to cap PID output at
    #  @param max Maximum value to cap PID output at
    def __init__(self, kp = 0.0, ki = 0.0, kd = 0.0, kf = 0.0, min = -1.0, max = 1.0):
        self._ptr = bridge.arpirobot.PID_create(kp, ki, kd, kf, min, max)
    
    ## Get the proportional gain
    #  @return proportional gain (kp)
    def get_kp(self) -> float:
        return bridge.arpirobot.PID_getKp(self._ptr)
    
    ## Set the proportional gain
    #  @param kp New value of kp
    def set_kp(self, kp: float):
        bridge.arpirobot.PID_setKp(self._ptr, kp)
    
    ## Get the integral gain
    #  @return integral gain (ki)
    def get_ki(self) -> float:
        return bridge.arpirobot.PID_getKi(self._ptr)
    
    ## Set the integral gain
    #  @param ki New value of ki
    def set_ki(self, ki: float):
        bridge.arpirobot.PID_setKi(self._ptr, ki)
    
    ## Get the derivative gain
    #  @return derivative gain (kd)
    def get_kd(self) -> float:
        return bridge.arpirobot.PID_getKd(self._ptr)
    
    ## Set the derivative gain
    #  @param kd New value of kd
    def set_kd(self, kd: float):
        bridge.arpirobot.PID_setKd(self._ptr, kd)
    
    ## Get the feedforward gain
    #  @return feedforward gain (kf)
    def get_kf(self) -> float:
        return bridge.arpirobot.PID_getKf(self._ptr)
    
    ## Set the feedforward gain
    #  @param kf New value of kf
    def set_kf(self, kf: float) -> float:
        bridge.arpirobot.PID_setKf(self._ptr, kf)
    
    ## Get minimum allowed PID output
    #  @return min allowed PID output
    def get_min(self) -> float:
        return bridge.arpirobot.PID_getMin(self._ptr)
    
    ## Set minimum allowed PID output
    #  @param min New min allowed PID output
    def set_min(self, min: float):
        bridge.arpirobot.PID_setMin(self._ptr, min)
    
    ## Get maximum allowed PID output
    #  @return max allowed PID output
    def get_max(self) -> float:
        return bridge.arpirobot.PID_getMax(self._ptr)
    
    ## Set maximum allowed PID output
    #  @param max New max allowed PID output
    def set_max(self, max: float):
        bridge.arpirobot.PID_setMax(self._ptr, max)
    
    ## Get the controller's current setpoint
    #  @return Current setpoint
    def get_setpoint(self) -> float:
        return bridge.arpirobot.PID_getSetpoint(self._ptr)
    
    ## Set the controller's setpoint
    #  @param setpoint New setpoint for the controller
    def set_setpoint(self, setpoint: float):
        bridge.arpirobot.PID_setSetpoint(self._ptr, setpoint)
    
    ## Reset the PID controller's internal state.
    #  This resets the accumulated integral value and the previous error
    #  used by the derivative gain.
    def reset(self):
        bridge.arpirobot.PID_reset(self._ptr)
    
    ## Calculate the output of the PID controller.
    #  @param current_pv Current value of the process variable
    #  @return The PID controller's output, capped at the set min and max values
    def get_output(self, current_pv: float) -> float:
        return bridge.arpirobot.PID_getOutput(self._ptr, current_pv)
