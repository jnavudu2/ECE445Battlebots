import pygame
import pymunk
import pymunk.pygame_util
import sys
import math
from connect import initialize_joystick, get_joystick_input

pygame.init()

WIDTH, HEIGHT = 1200, 900
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Battlebot Simulation")

space = pymunk.Space()
space.gravity = (0, 0)

# Robot properties
robot_width = 50
robot_height = 80
mass = 10
size = (robot_width, robot_height)
moment = pymunk.moment_for_box(mass, size) + \
         pymunk.moment_for_box(mass * 0.3, (size[0] * 0.8, size[1] * 0.3))
max_speed = 600  # Maximum speed in pixels per second
max_angular_speed = 6  # Maximum angular speed in radians per second

body = pymunk.Body(mass, moment)
body.position = WIDTH // 2, HEIGHT // 2

def apply_tank_drive(left_motor, right_motor):
    left_speed = (left_motor / 127) * max_speed
    right_speed = (right_motor / 127) * max_speed
    
    # Calculate linear and angular velocity
    linear_velocity = (left_speed + right_speed) / 2
    angular_velocity = (left_speed - right_speed) / robot_height
    angular_velocity = max(min(angular_velocity, max_angular_speed), -max_angular_speed)
    
    # Set angular velocity to 0 if the difference between motor speeds is very small
    if abs(left_speed - right_speed) < 0.01 * max_speed:
        angular_velocity = 0
    
    body.velocity = (linear_velocity * math.cos(body.angle), linear_velocity * math.sin(body.angle))
    body.angular_velocity = angular_velocity

def draw_robot():
    # Create a simple robot body
    shape = pymunk.Poly.create_box(body, size)
    shape.color = pygame.Color("firebrick1")
    space.add(body, shape)

    # Add weapon
    weapon = pymunk.Segment(body, (robot_width / 2, robot_height/2-15), (robot_width/2, -robot_height/2+15), 8)
    weapon.color = pygame.Color("firebrick3")
    space.add(weapon)

draw_robot()
draw_options = pymunk.pygame_util.DrawOptions(screen)

clock = pygame.time.Clock()

joystick = initialize_joystick()
if joystick is None:
    pygame.quit()
    sys.exit()

while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()
    
    left_motor, right_motor, weapon_on, arm_1, arm_2 = get_joystick_input(joystick)
    
    # Apply tank drive control
    if (arm_1 > 0.5 and arm_2 > 0.5):
        apply_tank_drive(left_motor, right_motor)
    
    space.step(1/60)
    
    screen.fill((255, 255, 255))
    
    space.debug_draw(draw_options)
    
    pygame.display.flip()
    
    clock.tick(60)
