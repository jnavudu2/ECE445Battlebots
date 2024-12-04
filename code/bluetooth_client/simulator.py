import sys
import math
import pygame
import pymunk
import pymunk.pygame_util
from connect import initialize_joystick, get_joystick_input, scale_weapon_speed

# Initialize Pygame and Pymunk
pygame.init()
pygame.freetype.init()

# Screen dimensions and setup
WIDTH, HEIGHT = 1200, 900
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Battlebot Simulation")

# Space setup
space = pymunk.Space()
space.gravity = (0, 0)
draw_options = pymunk.pygame_util.DrawOptions(screen)
clock = pygame.time.Clock()
CLOCK_FREQ = 60

# Font setup
status_font = pygame.freetype.SysFont('ttinterphasespromonotrlregular', 24)

# Robot properties
ROBOT_WIDTH = 30
ROBOT_HEIGHT = 80
ROBOT_MASS = 10
ROBOT_SIZE = (ROBOT_WIDTH, ROBOT_HEIGHT)
MASS_CENTER = pymunk.moment_for_box(ROBOT_MASS, ROBOT_SIZE)
MASS_TOP = pymunk.moment_for_box(ROBOT_MASS * 0.3, (ROBOT_WIDTH * 0.8, ROBOT_HEIGHT * 0.3))
ROBOT_MOMENT = MASS_CENTER + MASS_TOP
MAX_SPEED = 1200
MAX_ANGULAR_SPEED = 7

# Global state variables
left_motor = 0
right_motor = 0
weapon_percentage = 0
arm_1 = 0
arm_2 = 0

# Create robot body
body = pymunk.Body(ROBOT_MASS, ROBOT_MOMENT)
body.position = WIDTH // 2, HEIGHT // 2

def check_bounds(body, width, height):
    """
    Ensures a body stays within the specified bounds of a window/screen.

    Args:
        body: A pymunk.Body object representing the physical body to check
        width (int): The maximum x-coordinate boundary
        height (int): The maximum y-coordinate boundary

    Returns:
        None. Body position is modified in place if out of bounds.

    Note:
        - x position is clamped between 0 and width 
        - y position is clamped between 0 and height
        - Modifies the body's position directly if it exceeds boundaries
    """
    x, y = body.position
    x = max(0, min(x, width))
    y = max(0, min(y, height))
    body.position = (x, y)

def apply_tank_drive(left_motor, right_motor):
    """
    Simulates tank drive motion for a robot based on left and right motor inputs.
    This function calculates the linear and angular velocities of the robot using differential drive
    kinematics and applies them to the robot's physics body. It also ensures the robot stays within
    the simulation boundaries.
    Args:
        left_motor (int): Left motor value ranging from -127 to 127, where positive values move forward
        right_motor (int): Right motor value ranging from -127 to 127, where positive values move forward
    Notes:
        - Uses global constants MAX_SPEED, ROBOT_HEIGHT, and MAX_ANGULAR_SPEED
        - Modifies global 'body' object's velocity and angular velocity
        - Calls check_bounds() to ensure robot stays in simulation area
        - Small differences between motor speeds (< 1% of MAX_SPEED) are treated as straight motion
    Returns:
        None
    """
    left_speed = (left_motor / 127) * MAX_SPEED
    right_speed = (right_motor / 127) * MAX_SPEED

    linear_velocity = (left_speed + right_speed) / 2
    angular_velocity = (left_speed - right_speed) / ROBOT_HEIGHT
    angular_velocity = max(min(angular_velocity, MAX_ANGULAR_SPEED), -MAX_ANGULAR_SPEED)

    if abs(left_speed - right_speed) < 0.01 * MAX_SPEED:
        angular_velocity = 0

    body.velocity = (linear_velocity * math.cos(body.angle), linear_velocity * math.sin(body.angle))
    body.angular_velocity = angular_velocity

    check_bounds(body, WIDTH, HEIGHT)

def draw_robot():
    """
    Creates and adds the robot's physical components to the physics simulation space.
    The robot consists of several parts:
    - Main body: A rectangular box with cadetblue1 color
    - Weapon: A rectangular protrusion on the right side (firebrick1 color)
    - Behind weapon: A black barrier behind the weapon
    - Wheels: Two rectangular wheels on the sides (firebrick1 color)
    - Bumpers: Two black protective strips above and below the wheels
    Each component is created as a Pymunk polygon shape attached to the robot's body
    and added to the physics space.
    Global variables used:
        - body: The Pymunk body object representing the robot
        - space: The Pymunk space where physics simulation occurs
        - ROBOT_SIZE: Tuple containing robot dimensions
        - ROBOT_WIDTH: Width of the robot
        - ROBOT_HEIGHT: Height of the robot
    """
    shape = pymunk.Poly.create_box(body, ROBOT_SIZE)
    shape.color = pygame.Color("cadetblue1")
    space.add(body, shape)

    weapon = pymunk.Poly(body, [
        (ROBOT_WIDTH / 2 - 25,  ROBOT_HEIGHT / 2 - 36),
        (ROBOT_WIDTH / 2 - 25, -ROBOT_HEIGHT / 2 + 36),
        (ROBOT_WIDTH / 2 + 10, -ROBOT_HEIGHT / 2 + 36),
        (ROBOT_WIDTH / 2 + 10,  ROBOT_HEIGHT / 2 - 36)
    ])
    weapon.color = pygame.Color("firebrick1")
    space.add(weapon)

    behind_weapon = pymunk.Poly(body, [
        (ROBOT_WIDTH / 2 - 30,  ROBOT_HEIGHT / 2 - 36),
        (ROBOT_WIDTH / 2 - 30, -ROBOT_HEIGHT / 2 + 36),
        (ROBOT_WIDTH / 2 - 25, -ROBOT_HEIGHT / 2 + 36),
        (ROBOT_WIDTH / 2 - 25,  ROBOT_HEIGHT / 2 - 36)
    ])
    behind_weapon.color = pygame.Color("black")
    space.add(behind_weapon)

    right_wheel = pymunk.Poly(body, [
        (ROBOT_WIDTH / 2 - ROBOT_WIDTH, ROBOT_HEIGHT / 2),
        (ROBOT_WIDTH / 2 - ROBOT_WIDTH, ROBOT_HEIGHT / 2 - 7),
        (ROBOT_WIDTH / 2, ROBOT_HEIGHT / 2 - 7),
        (ROBOT_WIDTH / 2, ROBOT_HEIGHT / 2)
    ])
    right_wheel.color = pygame.Color("firebrick1")
    space.add(right_wheel)

    left_wheel = pymunk.Poly(body, [
        (ROBOT_WIDTH / 2 - ROBOT_WIDTH, ROBOT_HEIGHT / 2 - ROBOT_HEIGHT),
        (ROBOT_WIDTH / 2 - ROBOT_WIDTH, ROBOT_HEIGHT / 2 + 7 - ROBOT_HEIGHT),
        (ROBOT_WIDTH / 2, ROBOT_HEIGHT / 2 + 7 - ROBOT_HEIGHT),
        (ROBOT_WIDTH / 2, ROBOT_HEIGHT / 2 - ROBOT_HEIGHT)
    ])
    left_wheel.color = pygame.Color("firebrick1")
    space.add(left_wheel)

    right_bumper = pymunk.Poly(body, [
        (ROBOT_WIDTH / 2 - ROBOT_WIDTH, ROBOT_HEIGHT / 2),
        (ROBOT_WIDTH / 2 - ROBOT_WIDTH, ROBOT_HEIGHT / 2 + 3),
        (ROBOT_WIDTH / 2, ROBOT_HEIGHT / 2 + 3),
        (ROBOT_WIDTH / 2, ROBOT_HEIGHT / 2)
    ])
    right_bumper.color = pygame.Color("black")
    space.add(right_bumper)

    left_bumper = pymunk.Poly(body, [
        (ROBOT_WIDTH / 2 - ROBOT_WIDTH, ROBOT_HEIGHT / 2 - ROBOT_HEIGHT),
        (ROBOT_WIDTH / 2 - ROBOT_WIDTH, ROBOT_HEIGHT / 2 - 3 - ROBOT_HEIGHT),
        (ROBOT_WIDTH / 2, ROBOT_HEIGHT / 2 - 3 - ROBOT_HEIGHT),
        (ROBOT_WIDTH / 2, ROBOT_HEIGHT / 2 - ROBOT_HEIGHT)
    ])
    left_bumper.color = pygame.Color("black")
    space.add(left_bumper)

def draw_status_box(screen):
    """Draws a status box on the pygame screen displaying robot control information.
    The status box shows:
    - Left motor speed
    - Right motor speed 
    - Weapon percentage
    - Arm 1 status (ARMED/DISARMED)
    - Arm 2 status (ARMED/DISARMED)
    Args:
        screen: Pygame screen surface to draw the status box on
    The box is positioned in the top right corner with fixed dimensions and displays
    real-time values of the robot's controls using the global variables:
    left_motor, right_motor, weapon_percentage, arm_1, and arm_2.
    Returns:
        None
    """
    box_width, box_height, box_margin = 280, 180, 20
    box_rect = pygame.Rect(WIDTH - box_width - box_margin, box_margin, box_width, box_height)

    pygame.draw.rect(screen, pygame.Color("gray71"), box_rect)
    pygame.draw.rect(screen, pygame.Color("gray28"), box_rect, 2)

    y_offset = box_margin + 10
    text_color = pygame.Color("black")

    status_font.render_to(screen, (WIDTH - box_width - box_margin + 10, y_offset),
                          f"Left Motor:  {left_motor:.2f}", text_color)
    
    status_font.render_to(screen, (WIDTH - box_width - box_margin + 10, y_offset + 35),
                          f"Right Motor: {right_motor:.2f}", text_color)
    
    status_font.render_to(screen, (WIDTH - box_width - box_margin + 10, y_offset + 70),
                          f"Weapon:      {weapon_percentage:.1f}%", text_color)
    
    arm_status_1 = "ARMED" if arm_1 > 0.5 else "DISARMED"
    arm_color_1 = pygame.Color("red") if arm_status_1 == "ARMED" else pygame.Color("green")
    status_font.render_to(screen, (WIDTH - box_width - box_margin + 10, y_offset + 105),
                          f"Arm 1:   {arm_status_1:>6}", arm_color_1)
    
    arm_status_2 = "ARMED" if arm_2 > 0.5 else "DISARMED"
    arm_color_2 = pygame.Color("red") if arm_status_2 == "ARMED" else pygame.Color("green")
    status_font.render_to(screen, (WIDTH - box_width - box_margin + 10, y_offset + 140),
                          f"Arm 2:   {arm_status_2:>6}", arm_color_2)

def draw_arena(screen):
    """
    Draw the arena on the Pygame screen with specified dimensions and visual elements.
    This function creates a rectangular arena with borders, a center circle, and cross lines.
    The arena consists of:
    - A gray background
    - Dark gray borders on all sides
    - A red circle in the center
    - Two perpendicular lines crossing at the center
    Args:
        screen (pygame.Surface): The Pygame surface to draw the arena on. Must be initialized
            with proper WIDTH and HEIGHT values.
    Returns:
        None
    Note:
        Global constants WIDTH and HEIGHT must be defined before calling this function.
    """
    arena_color = pygame.Color("gray71")
    border_color = pygame.Color("gray28")
    circle_color = pygame.Color("red2")
    pygame.draw.rect(screen, arena_color, (0, 0, WIDTH, HEIGHT))

    border_width = 20

    pygame.draw.rect(screen, border_color, (0, 0, WIDTH, border_width))
    pygame.draw.rect(screen, border_color, (0, HEIGHT - border_width, WIDTH, border_width))
    pygame.draw.rect(screen, border_color, (0, 0, border_width, HEIGHT))
    pygame.draw.rect(screen, border_color, (WIDTH - border_width, 0, border_width, HEIGHT))

    pygame.draw.circle(screen, circle_color, (WIDTH // 2, HEIGHT // 2), 100, 5)

    pygame.draw.line(screen, border_color, (WIDTH // 2, 0), (WIDTH // 2, HEIGHT), 5)
    pygame.draw.line(screen, border_color, (0, HEIGHT // 2), (WIDTH, HEIGHT // 2), 5)

joystick = initialize_joystick()
if joystick is None:
    pygame.quit()
    sys.exit()

draw_robot()

# Main loop
while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

    left_motor, right_motor, weapon_speed, arm_1, arm_2 = get_joystick_input(joystick)
    weapon_percentage = (scale_weapon_speed(weapon_speed) / scale_weapon_speed(1)) * 100
    if arm_1 < 0.5 or arm_2 < 0.5:
        weapon_percentage = 0
    if arm_2 < 0.5:
        left_motor = 0
        right_motor = 0
    apply_tank_drive(left_motor, right_motor)

    space.step(1 / CLOCK_FREQ)
    
    screen.fill(pygame.Color("white"))
    
    draw_arena(screen)
    
    space.debug_draw(draw_options)
    
    draw_status_box(screen)
    
    pygame.display.flip()
    
    clock.tick(CLOCK_FREQ)
