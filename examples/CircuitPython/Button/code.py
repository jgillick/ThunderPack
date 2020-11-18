import digitalio
import board

# Setup the button and LED
btn = digitalio.DigitalInOut(board.BUTTON)
btn.direction = digitalio.Direction.INPUT

led = digitalio.DigitalInOut(board.LED1)
led.direction = digitalio.Direction.OUTPUT

# Constantly check the button value
while True:
  # Set the LED to the value of the button
  led.value = btn.value
