import digitalio
from board import *
import time

# Pin list
leds = [
    digitalio.DigitalInOut(LED1),
    digitalio.DigitalInOut(LED2),
    digitalio.DigitalInOut(LED3),
    digitalio.DigitalInOut(LED4),
]
btn = digitalio.DigitalInOut(PB04)
btn.direction = digitalio.Direction.INPUT

# Set LEDs to output
for l in leds:
    l.direction = digitalio.Direction.OUTPUT

# Loop through LEDs
i = 0
while True:
    leds[i].value = not leds[i].value
    
    # Faster when button pressed
    if btn.value:
        time.sleep(.05)
    else:
        time.sleep(.150)
    
    # Increment
    i += 1
    i = i % len(leds) # wrap around back to 0 when we get to the end
    