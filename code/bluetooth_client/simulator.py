import pygame
import pymunk
import pymunk.pygame_util
import sys
import math
from connect import initialize_joystick, get_joystick_input, scale_weapon_speed

ROBOT_COLLISION_TYPE = 1
OBJECT_COLLISION_TYPE = 2

pygame.init()

WIDTH, HEIGHT = 1200, 900
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Battlebot Simulation")

space = pymunk.Space()
space.gravity = (0, 0)

# Robot properties
robot_width = 30
robot_height = 80
mass = 10
size = (robot_width, robot_height)
moment = pymunk.moment_for_box(mass, size) + \
         pymunk.moment_for_box(mass * 0.3, (size[0] * 0.8, size[1] * 0.3))
max_speed = 1200  # Maximum speed in pixels per second
max_angular_speed = 7  # Maximum angular speed in radians per second
weapon_percentage = 0

# Create robot physics
body = pymunk.Body(mass, moment)
body.position = WIDTH // 2, HEIGHT // 2

def check_bounds(body, width, height):
    x, y = body.position
    if x < 0:
        x = 0
    elif x > width:
        x = width

    if y < 0:
        y = 0
    elif y > height:
        y = height

    body.position = (x, y)

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
    
    # Apply the boundary check
    check_bounds(body, WIDTH, HEIGHT)

def draw_robot():
    # Create a simple robot body
    shape = pymunk.Poly.create_box(body, size)
    shape.color = pygame.Color("cadetblue1")
    space.add(body, shape)

    # Add weapon
    weapon = pymunk.Poly(body, [(robot_width/2-25, robot_height/2-36), 
                                (robot_width/2-25, -robot_height/2+36), 
                                (robot_width/2+10, -robot_height/2+36), 
                                (robot_width/2+10, robot_height/2-36)])
    weapon.color = pygame.Color("firebrick1")
    space.add(weapon)

    behind_weapon = pymunk.Poly(body, [(robot_width/2-30, robot_height/2-36),
                                (robot_width/2-30, -robot_height/2+36),
                                (robot_width/2-25, -robot_height/2+36),
                                (robot_width/2-25, robot_height/2-36)])
    behind_weapon.color = pygame.Color("black")
    space.add(behind_weapon)

    # Add wheels
    right_wheel = pymunk.Poly(body, [(robot_width/2-robot_width, robot_height/2),
                                    (robot_width/2-robot_width, robot_height/2-7),
                                    (robot_width/2, robot_height/2-7),
                                    (robot_width/2, robot_height/2)])
    right_wheel.color = pygame.Color("firebrick1")
    space.add(right_wheel)

    left_wheel = pymunk.Poly(body, [(robot_width/2-robot_width, robot_height/2 - robot_height),
                                    (robot_width/2-robot_width, robot_height/2+7 - robot_height),
                                    (robot_width/2, robot_height/2+7 - robot_height),
                                    (robot_width/2, robot_height/2 - robot_height)])
    left_wheel.color = pygame.Color("firebrick1")
    space.add(left_wheel)

    # Add outer bumpers
    right_bumper = pymunk.Poly(body, [(robot_width/2-robot_width, robot_height/2),
                                    (robot_width/2-robot_width, robot_height/2+3),
                                    (robot_width/2, robot_height/2+3),
                                    (robot_width/2, robot_height/2)])
    right_bumper.color = pygame.Color("black")
    space.add(right_bumper)

    left_bumper = pymunk.Poly(body, [(robot_width/2-robot_width, robot_height/2 - robot_height),
                                    (robot_width/2-robot_width, robot_height/2-3 - robot_height),
                                    (robot_width/2, robot_height/2-3 - robot_height),
                                    (robot_width/2, robot_height/2 - robot_height)])
    left_bumper.color = pygame.Color("black")
    space.add(left_bumper)

def draw_status_box(screen, left_motor, right_motor, weapon_on, arm_1, arm_2):
     # Create status box background
    box_width = 280
    box_height = 180
    box_margin = 20
    box_rect = pygame.Rect(WIDTH - box_width - box_margin, box_margin, box_width, box_height)
    pygame.draw.rect(screen, (200, 200, 200), box_rect)
    pygame.draw.rect(screen, (100, 100, 100), box_rect, 2)

    # Draw status text
    y_offset = box_margin + 10
    text_color = (0, 0, 0)
    
    # Left Motor
    status_font.render_to(screen, 
                         (WIDTH - box_width - box_margin + 10, y_offset),
                         f"Left Motor:  {left_motor:.2f}", text_color)
    
    # Right Motor
    status_font.render_to(screen, 
                         (WIDTH - box_width - box_margin + 10, y_offset + 35),
                         f"Right Motor: {right_motor:.2f}", text_color)
    
    # Weapon Status
    status_font.render_to(screen, 
                         (WIDTH - box_width - box_margin + 10, y_offset + 70),
                         f"Weapon:      {weapon_percentage:.1f}%", text_color)
    
    # Arm Status 1
    arm_status_1 = "ARMED" if arm_1 > 0.5 else "DISARMED"
    arm_status_1_color = pygame.Color("red") if arm_status_1 == "ARMED" else pygame.Color("green")
    status_font.render_to(screen,
                        (WIDTH - box_width - box_margin + 10, y_offset + 105),
                        f"Arm 1:   {arm_status_1:>6}", arm_status_1_color)
    
    # Arm Status 2
    arm_status_2 = "ARMED" if arm_2 > 0.5 else "DISARMED"
    arm_status_2_color = pygame.Color("red") if arm_status_2 == "ARMED" else pygame.Color("green")
    status_font.render_to(screen,
                        (WIDTH - box_width - box_margin + 10, y_offset + 140),
                        f"Arm 2:   {arm_status_2:>6}", arm_status_2_color)
    
def draw_arena(screen):
    # Set arena colors
    arena_color = (200, 200, 200)  # Light gray
    border_color = (50, 50, 50)  # Dark gray
    circle_color = (200, 0, 0) # Red
    
    # Draw the main arena floor
    pygame.draw.rect(screen, arena_color, (0, 0, WIDTH, HEIGHT))
    
    # Draw arena borders
    border_width = 20
    pygame.draw.rect(screen, border_color, (0, 0, WIDTH, border_width))  # Top border
    pygame.draw.rect(screen, border_color, (0, HEIGHT - border_width, WIDTH, border_width))  # Bottom border
    pygame.draw.rect(screen, border_color, (0, 0, border_width, HEIGHT))  # Left border
    pygame.draw.rect(screen, border_color, (WIDTH - border_width, 0, border_width, HEIGHT))  # Right border
    
    # Draw some arena details
    pygame.draw.circle(screen, circle_color, (WIDTH // 2, HEIGHT // 2), 100, 5)  # Center circle
    pygame.draw.line(screen, border_color, (WIDTH // 2, 0), (WIDTH // 2, HEIGHT), 5)  # Vertical line
    pygame.draw.line(screen, border_color, (0, HEIGHT // 2), (WIDTH, HEIGHT // 2), 5)  # Horizontal line
    
pygame.freetype.init()
status_font = pygame.freetype.SysFont('ttinterphasespromonotrlregular', 24)

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
    
    left_motor, right_motor, weapon_speed, arm_1, arm_2 = get_joystick_input(joystick)

    weapon_percentage = (scale_weapon_speed(weapon_speed) / scale_weapon_speed(1)) * 100
    if arm_1 < 0.5 or arm_2 < 0.5:
        weapon_percentage = 0
    
    # Apply tank drive control
    if (arm_2 > 0.5):
        apply_tank_drive(left_motor, right_motor)
    else:
        apply_tank_drive(0, 0)
    
    space.step(1/60)
    
    screen.fill((255, 255, 255))

    draw_arena(screen)
    
    space.debug_draw(draw_options)

    draw_status_box(screen, left_motor, right_motor, weapon_speed, arm_1, arm_2)
    
    pygame.display.flip()
    
    clock.tick(60)
