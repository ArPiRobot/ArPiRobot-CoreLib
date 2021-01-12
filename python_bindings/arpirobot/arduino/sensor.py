
import arpirobot.bridge as bridge
import ctypes
from typing import Union
from arpirobot.arduino.device import ArduinoDevice


class VoltageMonitor(ArduinoDevice):
    def __init__(self, pin: Union[int, str], vboard: float, r1: int, r2: int, 
            create_device: bool = True, device_id: int = -1):
        super().__init__()
        self._ptr = bridge.arpirobot.VoltageMonitor_create(str(pin).encode(), vboard, r1, r2, create_device, device_id)
    
    def __del__(self):
        bridge.arpirobot.VoltageMonitor_destroy(self._ptr)
    
    def get_voltage(self) -> float:
        return bridge.arpirobot.VoltageMonitor_getVoltage(self._ptr)
    
    def make_main_vmon(self):
        bridge.arpirobot.VoltageMonitor_makeMainVmon(self._ptr)


class Ultrasonic4Pin(ArduinoDevice):
    def __init__(self, trigger_pin: Union[str, int], echo_pin: Union[str, int], 
            create_device: bool = True, device_id : int = -1):
        super().__init__()
        self._ptr = bridge.arpirobot.Ultrasonic4Pin_create(str(trigger_pin).encode(), str(echo_pin).encode(), 
            create_device, device_id)
    
    def __del__(self):
        bridge.arpirobot.Ultrasonic4Pin_destroy(self._ptr)
    
    def get_distance(self) -> int:
        return bridge.arpirobot.Ultrasonic4Pin_getDistance(self._ptr)


class SingleEncoder(ArduinoDevice):
    def __init__(self, pin: Union[int, str], use_internal_pullup: bool, create_device = True, device_id = -1):
        super().__init__()
        self._ptr = bridge.arpirobot.SingleEncoder_create(str(pin).encode(), use_internal_pullup, 
            create_device, device_id)
    
    def __del__(self):
        bridge.arpirobot.SingleEncoder_destroy(self._ptr)
    
    def get_position(self) -> int:
        return bridge.arpirobot.SingleEncoder_getPosition(self._ptr)
    
    def set_position(self, new_position: int):
        bridge.arpirobot.SingleEncoder_setPosition(self._ptr, new_position)


class IRReflectorModule(ArduinoDevice):
    def __init__(self, digital_pin: Union[int, str], analog_pin: str = "", create_device = True, device_id = -1):
        super().__init__()
        self._ptr = bridge.arpirobot.IRReflectorModule_create(str(digital_pin).encode(), analog_pin.encode(), 
            create_device, device_id)
    
    def __del__(self):
        bridge.arpirobot.IRReflectorModule_destroy(self._ptr)
    
    def get_digital_value(self) -> bool:
        return bridge.arpirobot.IRReflectorModule_getDigitalValue(self._ptr)
    
    def get_analog_value(self) -> int:
        return bridge.arpirobot.IRReflectorModule_getAnalogValue(self._ptr)


class OldAdafruit9Dof(ArduinoDevice):
    def __init__(self, create_device = True, device_id = -1):
        super().__init__()
        self._ptr = bridge.arpirobot.OldAdafruit9Dof_create(create_device, device_id)
    
    def __del__(self):
        bridge.arpirobot.OldAdafruit9Dof_destroy(self._ptr)
    
    def get_gyro_x(self) -> float:
        return bridge.arpirobot.OldAdafruit9Dof_getGyroX(self._ptr)
    
    def get_gyro_y(self) -> float:
        return bridge.arpirobot.OldAdafruit9Dof_getGyroY(self._ptr)

    def get_gyro_z(self) -> float:
        return bridge.arpirobot.OldAdafruit9Dof_getGyroZ(self._ptr)

    def get_accel_x(self) -> float:
        return bridge.arpirobot.OldAdafruit9Dof_getAccelX(self._ptr)

    def get_accel_y(self) -> float:
        return bridge.arpirobot.OldAdafruit9Dof_getAccelY(self._ptr)

    def get_accel_z(self) -> float:
        return bridge.arpirobot.OldAdafruit9Dof_getAccelZ(self._ptr)

    def set_gyro_x(self, new_gyro_x: float):
        bridge.arpirobot.OldAdafruit9Dof_setGyroX(self._ptr, new_gyro_x)

    def set_gyro_y(self, new_gyro_y: float):
        bridge.arpirobot.OldAdafruit9Dof_setGyroY(self._ptr, new_gyro_y)

    def set_gyro_z(self, new_gyro_z: float):
        bridge.arpirobot.OldAdafruit9Dof_setGyroZ(self._ptr, new_gyro_z)


class NxpAdafruit9Dof(ArduinoDevice):
    def __init__(self, create_device = True, device_id = -1):
        super().__init__()
        self._ptr = bridge.arpirobot.NxpAdafruit9Dof_create(create_device, device_id)
    
    def __del__(self):
        bridge.arpirobot.NxpAdafruit9Dof_destroy(self._ptr)
    
    def get_gyro_x(self) -> float:
        return bridge.arpirobot.NxpAdafruit9Dof_getGyroX(self._ptr)
    
    def get_gyro_y(self) -> float:
        return bridge.arpirobot.NxpAdafruit9Dof_getGyroY(self._ptr)

    def get_gyro_z(self) -> float:
        return bridge.arpirobot.NxpAdafruit9Dof_getGyroZ(self._ptr)

    def get_accel_x(self) -> float:
        return bridge.arpirobot.NxpAdafruit9Dof_getAccelX(self._ptr)

    def get_accel_y(self) -> float:
        return bridge.arpirobot.NxpAdafruit9Dof_getAccelY(self._ptr)

    def get_accel_z(self) -> float:
        return bridge.arpirobot.NxpAdafruit9Dof_getAccelZ(self._ptr)

    def set_gyro_x(self, new_gyro_x: float):
        bridge.arpirobot.NxpAdafruit9Dof_setGyroX(self._ptr, new_gyro_x)

    def set_gyro_y(self, new_gyro_y: float):
        bridge.arpirobot.NxpAdafruit9Dof_setGyroY(self._ptr, new_gyro_y)

    def set_gyro_z(self, new_gyro_z: float):
        bridge.arpirobot.NxpAdafruit9Dof_setGyroZ(self._ptr, new_gyro_z)