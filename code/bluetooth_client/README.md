# Client Programs

This directory contains the client programs that are used to communicate with the robot. It also has some additional files that can be used to test control of the robot.

## Connecting to the Robot

To connect to the robot, you must have Python installed as well as the `simplepyble` and `pygame` modules. You can install these modules by running the following command:

```bash
pip install simplepyble pygame
```

This connection script was designed to run on Windows, but it should work on other operating systems as well. WSL will not work due to the inability to access Bluetooth devices.

1. Turn on the robot and wait for the Bluetooth module to start up.
2. Power on the controller and plug it into the computer.
3. Select "USB Joystick (HID)" on the controller.
4. Make sure both arm switches are in the "off" position (down) and the weapon switch wheel is in the "off" position (left).
5. Run the client program `connect.py` to connect to the robot and begin transmitting controls. This program will automatically scan nearby BLE devices for the robot and connect to it.
6. Engage the arm switches to move the robot.

## Extra Programs

### `joystick_check.py`

This program allows you to display the axes and button status of a connected controller. It is useful for determining which axes you want to use for controlling the robot.

### `simulator.py`

This program is used to run a physics-based simulation of the robot. It is useful for testing control and driving algorithms without needing to connect to the robot.
