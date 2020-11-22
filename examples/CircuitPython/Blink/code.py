import time
import digitalio
import board

DELAY_SLOW_MS = 0.8
DELAY_FAST_MS = 0.2

# Setup LED and button
led = digitalio.DigitalInOut(board.LED1)
led.direction = digitalio.Direction.OUTPUT

btn = digitalio.DigitalInOut(board.BUTTON)
btn.direction = digitalio.Direction.INPUT

# Blink infinitely
while True:
  # Toggle LED value
  led.value = not led.value

  # Delay
  if btn.value == True: # button pressed
    time.sleep(DELAY_FAST_MS)
  else:
    time.sleep(DELAY_SLOW_MS)
