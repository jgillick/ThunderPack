import time
import board
from pulseio import PWMOut

FADE_SPEED     = 0.1  # Smaller number = faster
FADE_INCREMENT = 4000 # How much to adjust the brightness per cycle

PWM_FULL = 65535

# Setup LED
led = PWMOut(board.LED1, frequency=5000, duty_cycle=0)

# Fade infinitely
direction = 1
brightness = 0
while True:
  brightness += FADE_INCREMENT * direction

  # Switch directions when we get to the limits
  if direction < 0 and brightness <= 0:
    brightness = 0
    direction = 1
  elif direction > 0 and brightness >= PWM_FULL:
    brightness = PWM_FULL
    direction = -1

  # Set PWM value
  led.duty_cycle = brightness

  time.sleep(FADE_SPEED)
