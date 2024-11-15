import pygame
import simplepyble
import logging

# Device constants
DEVICE_NAME = "ESP32-BLE"
SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"

# Joystick constants
FORWARD_BACKWARD_AXIS = 1
LEFT_RIGHT_AXIS = 3
WEAPON_BUTTON = 1
JOYSTICK_THRESHOLD = 0.1
ARM_1_AXIS = 5
ARM_2_AXIS = 7

logging.basicConfig(level=logging.INFO, format="%(asctime)s [%(levelname)s] %(message)s")

# Implements "semi-constant curvature drive" algorithm https://ewpratten.com/blog/joystick-to-voltage/
def compute_motor_speeds(throttle, steering):
    L = 128 * (((throttle + abs(throttle) * steering) + (throttle + steering)) / 2)
    R = 128 * (((throttle - abs(throttle) * steering) + (throttle - steering)) / 2)
    m = max(abs(throttle), abs(steering))
    if m > 1.0:
        L /= m
        R /= m
    return (L, R)

def inverse_message(message):
    weapon_on = message[0]
    left_motor = int.from_bytes([message[1]], byteorder="big", signed=True)
    right_motor = int.from_bytes([message[2]], byteorder="big", signed=True)

    return weapon_on, left_motor, right_motor

def to_signed_byte(value):
    value = int(value)
    if value < -128 or value > 127:
        value = 127 if value > 127 else -128
    return value if value >= 0 else 256 + value

def create_message(forward_backwards, left_right, weapon):
    left_motor, right_motor = compute_motor_speeds(forward_backwards, left_right)
    return bytes([weapon, to_signed_byte(left_motor), to_signed_byte(right_motor)])

def select_adapter(adapters):
    if len(adapters) == 0:
        logging.error("No adapters found")
        return None
    elif len(adapters) == 1:
        logging.info(f"Found 1 adapter: {adapters[0].identifier()} [{adapters[0].address()}]")
        return adapters[0]
    else:
        logging.info("Please select an adapter:")
        for i, adapter in enumerate(adapters):
            logging.info(f"{i}: {adapter.identifier()} [{adapter.address()}]")
        choice = int(input("Enter choice: "))
        return adapters[choice]

def find_device(adapter):
    adapter.set_callback_on_scan_start(lambda: logging.info("Scan started."))
    adapter.set_callback_on_scan_stop(lambda: logging.info("Scan complete."))
    adapter.set_callback_on_scan_found(lambda peripheral: logging.info(f"Found {peripheral.identifier()} [{peripheral.address()}]"))
    adapter.scan_for(5000)
    peripherals = adapter.scan_get_results()
    for i, peripheral in enumerate(peripherals):
        if peripheral.identifier() == DEVICE_NAME:
            logging.info(f"Found {DEVICE_NAME} at address [{peripheral.address()}]")
            return peripherals[i]
    logging.error(f"Could not find device with name '{DEVICE_NAME}'")
    return None

def initialize_joystick():
    pygame.joystick.init()
    if pygame.joystick.get_count() == 0:
        logging.error("No joysticks found")
        return None
    joystick = pygame.joystick.Joystick(0)
    joystick.init()
    logging.info(f"Initialized joystick: {joystick.get_name()}")
    return joystick

# used for simulation
def get_joystick_input(joystick):
    forward_backwards = joystick.get_axis(FORWARD_BACKWARD_AXIS)
    left_right = joystick.get_axis(LEFT_RIGHT_AXIS)
    weapon = joystick.get_button(WEAPON_BUTTON)
    arm_1 = joystick.get_axis(ARM_1_AXIS)
    arm_2 = joystick.get_axis(ARM_2_AXIS)
    
    left_motor, right_motor = compute_motor_speeds(forward_backwards, left_right)

    return left_motor, right_motor, weapon, arm_1, arm_2

def main():
    adapters = simplepyble.Adapter.get_adapters()
    adapter = select_adapter(adapters)
    if not adapter:
        return

    logging.info(f"Selected adapter: {adapter.identifier()} [{adapter.address()}]")
    peripheral = find_device(adapter)
    if not peripheral:
        return

    logging.info(f"Connecting to: {peripheral.identifier()} [{peripheral.address()}]")
    peripheral.connect()
    logging.info("Successfully connected")

    pygame.init()
    joystick = initialize_joystick()
    if not joystick:
        return

    try:
        while True:
            pygame.event.pump()
            try:
                forward_backwards = joystick.get_axis(FORWARD_BACKWARD_AXIS)
                left_right = joystick.get_axis(LEFT_RIGHT_AXIS)
                weapon = joystick.get_button(WEAPON_BUTTON)
                arm_1 = joystick.get_axis(ARM_1_AXIS)
                arm_2 = joystick.get_axis(ARM_2_AXIS)
            except Exception as e:
                logging.error(f"Joystick error: {str(e)}")
                continue

            if arm_1 > 0.5 and arm_2 > 0.5:
                message = create_message(forward_backwards, left_right, weapon)
            else:
                message = create_message(0, 0, 0)

            try:
                peripheral.write_request(SERVICE_UUID, CHARACTERISTIC_UUID, message)
                weapon_on, left_motor, right_motor = inverse_message(message)
                logging.info(f"Sent: left_motor: {left_motor}, right_motor: {right_motor}, weapon: {weapon_on}")
            except Exception as e:
                logging.error(f"Bluetooth error: {str(e)}")

            # Wait to prevent spamming the device
            pygame.time.wait(100)
    except KeyboardInterrupt:
        logging.info("Exiting due to user interruption")

    logging.info("Disconnecting...")
    pygame.quit()
    peripheral.disconnect()

if __name__ == "__main__":
    main()
