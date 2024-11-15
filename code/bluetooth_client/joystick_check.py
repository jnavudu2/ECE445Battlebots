import pygame
import os
import time

def clear_console():
    os.system('cls' if os.name == 'nt' else 'clear')

pygame.init()
pygame.joystick.init()

if pygame.joystick.get_count() == 0:
    print("No joysticks found")
    pygame.quit()
    exit()

joystick = pygame.joystick.Joystick(0)
joystick.init()

print(f"Joystick Name: {joystick.get_name()}")
print(f"Number of axes: {joystick.get_numaxes()}")
print("Move the joystick to see which axes are activated.")
print("Press Ctrl+C to exit.")

try:
    while True:
        clear_console()
        pygame.event.pump()
        
        print(f"Joystick: {joystick.get_name()}")
        print("=" * 40)
        
        for i in range(joystick.get_numaxes()):
            axis_value = joystick.get_axis(i)
            bar_length = int(abs(axis_value) * 20)
            direction = "+" if axis_value > 0 else "-"
            bar = f"{'█' * bar_length:<20}"
            print(f"Axis {i:2d}: {bar} {axis_value:6.2f} {direction}")
        for i in range(joystick.get_numbuttons()):
            button_value = joystick.get_button(i)
            print(f"Button {i:2d}: {button_value}")
        
        print("=" * 40)
        print("Press Ctrl+C to exit")
        
        time.sleep(0.1)

except KeyboardInterrupt:
    print("\nExiting...")

pygame.quit()
