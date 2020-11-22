import time
import digitalio
from board import *

# Pin list
leds = [
    digitalio.DigitalInOut(LED1),
    digitalio.DigitalInOut(LED2),
    digitalio.DigitalInOut(LED3),
    digitalio.DigitalInOut(LED4),
]
btn = digitalio.DigitalInOut(BUTTON)
btn.direction = digitalio.Direction.INPUT

# Set all LEDs to output
for l in leds:
    l.direction = digitalio.Direction.OUTPUT

# Loop through LEDs
i = 0
num_leds = len(leds)
while True:
    leds[i].value = not leds[i].value

    # Faster when button pressed
    if btn.value:
        time.sleep(.05)
    else:
        time.sleep(.125)

    # Increment
    i += 1
    i = i % num_leds # wrap around back to 0 when we get to the end
