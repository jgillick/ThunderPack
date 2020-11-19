import time
import board
import adafruit_dotstar

# DotStar pins
# Change these if you want to connect an external DotStar LED(s) to different pins.
PIN_SCK = board.APA102_SCK
PIN_MOSI = board.APA102_MOSI

# The onboard RGB LED
led = adafruit_dotstar.DotStar(PIN_SCK, PIN_MOSI, 1, brightness=0.1)


def wheel(pos):
    # Input a value 0 to 255 to get a color value.
    # The colors are a transition r -> g -> b -> back to r.
    if pos < 0 or pos > 255:
        r = g = b = 0
    elif pos < 85:
        r = int(pos * 3)
        g = int(255 - pos * 3)
        b = 0
    elif pos < 170:
        pos -= 85
        r = int(255 - pos * 3)
        g = 0
        b = int(pos * 3)
    else:
        pos -= 170
        r = 0
        g = int(pos * 3)
        b = int(255 - pos * 3)
    return (r, g, b)


while True:
    # Animate the onboard LED around the color wheel
    for i in range(255):
        led[0] = wheel(i)
        time.sleep(0.001)