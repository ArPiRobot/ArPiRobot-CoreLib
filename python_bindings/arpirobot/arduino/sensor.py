
from arpirobot.core.network import MainVmon
import arpirobot.bridge as bridge
import ctypes
from typing import Union
from arpirobot.arduino.device import ArduinoDevice
from arpirobot.core.network import MainVmon


## Simple analog input voltage monitor (using voltage divider) connected to arduino analog input.
class VoltageMonitor(ArduinoDevice, MainVmon):
    ## @param pin The analog input pin this voltage monitor is connected to (can be prefixed with letter A)
    #  @param vboard THe board voltage for this arduino (usually 5V or 3.3V)
    #  @param r1 The top resistor of the voltage divider
    #  @param r2 The bottom resistor of the voltage divider (the one voltage is measured across)
    #  @param create_device Leave this true unless the device is hard-coded in arduino firmware
    #  @param device_id Set this to the hard-coded deviceId if createDevice is false
    def __init__(self, pin: Union[int, str], vboard: float, r1: int, r2: int, 
            create_device: bool = True, device_id: int = -1):
        super().__init__()
        self._ptr = bridge.arpirobot.VoltageMonitor_create(str(pin).encode(), vboard, r1, r2, create_device, device_id)
    
    def __del__(self):
        bridge.arpirobot.VoltageMonitor_destroy(self._ptr)
    
    ## Get the voltage read by this voltage monitor (in volts)
    #  @returns The voltage in volts
    def get_voltage(self) -> float:
        return bridge.arpirobot.VoltageMonitor_getVoltage(self._ptr)


## Device for 4-pin ultrasonic sensor.
class Ultrasonic4Pin(ArduinoDevice):
    ## @param trigger_pin Digital pin number for the trigger pin. If prefixed with "A" this is an analog pin
    #  @param echo_pin Digital pin number for the echo pin. If prefixed with "A" this is an analog pin
    #  @param create_device Leave this true unless the device is hard-coded in arduino firmware
    #  @param device_id Set this to the hard-coded deviceId if createDevice is false
    def __init__(self, trigger_pin: Union[str, int], echo_pin: Union[str, int], 
            create_device: bool = True, device_id : int = -1):
        super().__init__()
        self._ptr = bridge.arpirobot.Ultrasonic4Pin_create(str(trigger_pin).encode(), str(echo_pin).encode(), 
            create_device, device_id)
    
    def __del__(self):
        bridge.arpirobot.Ultrasonic4Pin_destroy(self._ptr)
    
    ## Get the last distance read by the ultrasonic sensor
    #  @returns The distance in cm
    def get_distance(self) -> int:
        return bridge.arpirobot.Ultrasonic4Pin_getDistance(self._ptr)


## Single-channel encoder
class SingleEncoder(ArduinoDevice):
    ## @param pin The digital pin number this encoder is connected to
    #  @param use_internal_pullup Set to true to use the arduino's internal pullup resistor on this pin (if supported)
    #  @param create_device Leave this true unless the device is hard-coded in arduino firmware
    #  @param device_id Set this to the hard-coded deviceId if createDevice is false
    def __init__(self, pin: Union[int, str], use_internal_pullup: bool, create_device = True, device_id = -1):
        super().__init__()
        self._ptr = bridge.arpirobot.SingleEncoder_create(str(pin).encode(), use_internal_pullup, 
            create_device, device_id)
    
    def __del__(self):
        bridge.arpirobot.SingleEncoder_destroy(self._ptr)
    
    ## Get the position (tick count) for this encoder
    #  @returns The position in ticks
    def get_position(self) -> int:
        return bridge.arpirobot.SingleEncoder_getPosition(self._ptr)
    
    ## Set the current tick count to the given value
    #  @param currentPosition The new tick value to set
    def set_position(self, new_position: int):
        bridge.arpirobot.SingleEncoder_setPosition(self._ptr, new_position)


## Infrared reflection detector module
class IRReflectorModule(ArduinoDevice):

    ## @param digital_pin The digital output pin for this sensor
    #  @param analog_pin The analog output pin for this sensor (can be prefixed with letter A, but not required)
    #                   To disable analog measurements leave this as an empty string (which is default)
    #  @param create_device Leave this true unless the device is hard-coded in arduino firmware
    #  @param device_id Set this to the hard-coded deviceId if createDevice is false
    def __init__(self, digital_pin: Union[int, str], analog_pin: str = "", create_device = True, device_id = -1):
        super().__init__()
        self._ptr = bridge.arpirobot.IRReflectorModule_create(str(digital_pin).encode(), analog_pin.encode(), 
            create_device, device_id)
    
    def __del__(self):
        bridge.arpirobot.IRReflectorModule_destroy(self._ptr)
    
    ## Get the digital value read from the sensor
    #  @returns true or false. One cooresponds to reflection detected. The other is no reflection detected.
    def get_digital_value(self) -> bool:
        return bridge.arpirobot.IRReflectorModule_getDigitalValue(self._ptr)
    
    ## Get the analog reflection signal strength (if this device was setup to use an analog input)
    #  @returns The read analog value (will be zero if not configured for analog data)
    def get_analog_value(self) -> int:
        return bridge.arpirobot.IRReflectorModule_getAnalogValue(self._ptr)


## Now discontinued Adafruit 9-DOF IMU
#  https://www.adafruit.com/product/1714
class OldAdafruit9Dof(ArduinoDevice):
    ## @param createDevice Leave this true unless the device is hard-coded in arduino firmware
    #  @param deviceId Set this to the hard-coded deviceId if createDevice is false
    def __init__(self, create_device = True, device_id = -1):
        super().__init__()
        self._ptr = bridge.arpirobot.OldAdafruit9Dof_create(create_device, device_id)
    
    def __del__(self):
        bridge.arpirobot.OldAdafruit9Dof_destroy(self._ptr)
    
    ## Get the X rotation
    #  @returns X rotation in degrees
    def get_gyro_x(self) -> float:
        return bridge.arpirobot.OldAdafruit9Dof_getGyroX(self._ptr)
    
    ## Get the Y rotation
    #  @returns Y rotation in degrees
    def get_gyro_y(self) -> float:
        return bridge.arpirobot.OldAdafruit9Dof_getGyroY(self._ptr)

    ## Get the Z rotation
    #  @returns Z rotation in degrees
    def get_gyro_z(self) -> float:
        return bridge.arpirobot.OldAdafruit9Dof_getGyroZ(self._ptr)

    ## Get the X axis acceleration
    #  @returns X acceleration in m/s^2
    def get_accel_x(self) -> float:
        return bridge.arpirobot.OldAdafruit9Dof_getAccelX(self._ptr)

    ## Get the Y axis acceleration
    #  @returns Y acceleration in m/s^2
    def get_accel_y(self) -> float:
        return bridge.arpirobot.OldAdafruit9Dof_getAccelY(self._ptr)

    ## Get the Z axis acceleration
    #  @returns Z acceleration in m/s^2
    def get_accel_z(self) -> float:
        return bridge.arpirobot.OldAdafruit9Dof_getAccelZ(self._ptr)

    ## Set current X rotation to the given value
    #  @param newGyroX The new X rotation to set
    def set_gyro_x(self, new_gyro_x: float):
        bridge.arpirobot.OldAdafruit9Dof_setGyroX(self._ptr, new_gyro_x)

    ## Set current Y rotation to the given value
    #  @param newGyroY The new Y rotation to set
    def set_gyro_y(self, new_gyro_y: float):
        bridge.arpirobot.OldAdafruit9Dof_setGyroY(self._ptr, new_gyro_y)

    ## Set current Z rotation to the given value
    #  @param newGyroZ The new Z rotation to set
    def set_gyro_z(self, new_gyro_z: float):
        bridge.arpirobot.OldAdafruit9Dof_setGyroZ(self._ptr, new_gyro_z)


## Newer adafruit 9-DOF NXP IMU
class NxpAdafruit9Dof(ArduinoDevice):
    def __init__(self, create_device = True, device_id = -1):
        super().__init__()
        self._ptr = bridge.arpirobot.NxpAdafruit9Dof_create(create_device, device_id)
    
    def __del__(self):
        bridge.arpirobot.NxpAdafruit9Dof_destroy(self._ptr)
    
    ## Get the X rotation
    #  @returns X rotation in degrees
    def get_gyro_x(self) -> float:
        return bridge.arpirobot.NxpAdafruit9Dof_getGyroX(self._ptr)
    
    ## Get the Y rotation
    #  @returns Y rotation in degrees
    def get_gyro_y(self) -> float:
        return bridge.arpirobot.NxpAdafruit9Dof_getGyroY(self._ptr)

    ## Get the Z rotation
    #  @returns Z rotation in degrees
    def get_gyro_z(self) -> float:
        return bridge.arpirobot.NxpAdafruit9Dof_getGyroZ(self._ptr)

    ## Get the X axis acceleration
    #  @returns X acceleration in m/s^2
    def get_accel_x(self) -> float:
        return bridge.arpirobot.NxpAdafruit9Dof_getAccelX(self._ptr)

    ## Get the Y axis acceleration
    #  @returns Y acceleration in m/s^2
    def get_accel_y(self) -> float:
        return bridge.arpirobot.NxpAdafruit9Dof_getAccelY(self._ptr)

    ## Get the Z axis acceleration
    #  @returns Z acceleration in m/s^2
    def get_accel_z(self) -> float:
        return bridge.arpirobot.NxpAdafruit9Dof_getAccelZ(self._ptr)

    ## Set current X rotation to the given value
    #  @param newGyroX The new X rotation to set
    def set_gyro_x(self, new_gyro_x: float):
        bridge.arpirobot.NxpAdafruit9Dof_setGyroX(self._ptr, new_gyro_x)

    ## Set current Y rotation to the given value
    #  @param newGyroY The new Y rotation to set
    def set_gyro_y(self, new_gyro_y: float):
        bridge.arpirobot.NxpAdafruit9Dof_setGyroY(self._ptr, new_gyro_y)

    ## Set current Z rotation to the given value
    #  @param newGyroZ The new Z rotation to set
    def set_gyro_z(self, new_gyro_z: float):
        bridge.arpirobot.NxpAdafruit9Dof_setGyroZ(self._ptr, new_gyro_z)