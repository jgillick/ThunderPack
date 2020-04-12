#
# Every time you press the button, the next LED turns on.
# When the board is turned off and back on, the previous LED will be lit up again.
#
from board import *
import microcontroller
import digitalio
import time

# Index of the memory slot we're using
MEMORY_IDX = 0

# Setup LEDs
leds = [
    digitalio.DigitalInOut(LED1),
    digitalio.DigitalInOut(LED2),
    digitalio.DigitalInOut(LED3),
    digitalio.DigitalInOut(LED4),
]
for led in leds:
    led.direction = digitalio.Direction.OUTPUT
    led.value = False

# Setup button
btn = digitalio.DigitalInOut(BUTTON)
btn.direction = digitalio.Direction.INPUT

# Get the last saved LED index from non-volatile memory (nvm)
current_idx = microcontroller.nvm[MEMORY_IDX]
if current_idx >= len(leds):
    current_idx = 0
leds[current_idx].value = True

# Handle button press
last_btn_value = False
while True:
    # Button pressed
    if (btn.value and last_btn_value == False):
        # Turn off previous LED
        leds[current_idx].value = False

        # Increment and wrap
        current_idx += 1
        current_idx = current_idx % len(leds)

        # Light next LED
        leds[current_idx].value = True

        # Save value to memory
        microcontroller.nvm[MEMORY_IDX] = current_idx

    last_btn_value = btn.value
    time.sleep(.1)
