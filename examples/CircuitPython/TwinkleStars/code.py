import time
import digitalio
import board
from pulseio import PWMOut
from random import randrange, uniform

PWM_FULL = 65535

class AnimationCtrl:
    """ A general-purpose class which controls the LED animations."""
    PHASE_PAUSE = 0
    PHASE_RUN = 1

    min_pause = 0
    max_pause = 0.5
    min_run = 0.1
    max_run = 0.2

    duration = 0.0
    phase = 0

    def __init__(self):
        pass

    def startRun(self):
        """Start an animation cycle."""
        self.phase = self.PHASE_RUN
        self.duration = uniform(self.min_run, self.max_run)

    def endRun(self):
        """End an animation cycle and pause."""
        self.phase = self.PHASE_PAUSE
        self.duration = uniform(self.min_pause, self.max_pause)

    def step(self, time):
        """Handle a single step in the animation.
        This should be called in each program loop."""
        self.duration -= time
        if self.duration <= 0:
            if self.phase == self.PHASE_PAUSE:
                self.startRun()
            else:
                self.endRun()


class FadeCtrl(AnimationCtrl):
    """Create random LED fade animations."""
    led = None
    inc = 0
    min_pause = 0.2
    max_pause = 5
    min_run = 0.5
    max_run = 1

    def __init__(self, pin):
        """Setup LED and start first fade animation."""
        self.led = PWMOut(pin, frequency=5000, duty_cycle=0)
        self.startRun()

    def startRun(self):
        """Start a new fade animation."""
        super().startRun()
        self.incPerMillisecond()

        if self.led.duty_cycle > 0:
            self.inc *= -1  # Decrement to zero

    def endRun(self):
        """The current fade animation is over."""
        super().endRun()

        # Finish on or off
        if self.inc > 0:
            self.led.duty_cycle = PWM_FULL
        else:
            self.led.duty_cycle = 0

        self.incPerMillisecond()

    def incPerMillisecond(self):
        """Calculate how much to increment the LED per millisecond
        to complete the fade animation by the end of the duration."""
        self.inc = PWM_FULL / self.duration

    def step(self, time):
        """Perform a single step of the animation. This should be called on each program loop."""
        if self.phase == self.PHASE_RUN:
            value = self.led.duty_cycle + int(self.inc * time)
            if value < 0:
                value = 0
            elif value > PWM_FULL:
                value = PWM_FULL
            self.led.duty_cycle = value
        super().step(time)


class TwinkleCtrl(AnimationCtrl):
    """Create a random 'twinkle' animation across two pins.
    This assumes you have fairy LED strings attached to the two pins.
    The pins will have one polarity for a random amount of time, and then swap.
    """
    pin1 = None
    pin2 = None
    min_pause = 0
    max_pause = 0.5
    min_run = 0.1
    max_run = 0.2

    def __init__(self, pin1, pin2):
        """Setup the LEDs and start the first animation."""
        super().__init__()

        self.pins = [
            digitalio.DigitalInOut(pin1),
            digitalio.DigitalInOut(pin2),
        ]
        for pin in self.pins:
            pin.direction = digitalio.Direction.OUTPUT
            pin.value = False

        self.startRun()

    def startRun(self):
        """Turn the LED on, in one polarity."""
        self.pins[1].value = not self.pins[0].value
        super().startRun()

    def endRun(self):
        """Turn the LEDs off."""
        self.pins[0].value = not self.pins[0].value
        super().endRun()

# Initialize all LED animations
animations = [
    TwinkleCtrl(board.PA9, board.PA13),
    TwinkleCtrl(board.PA14, board.PB5),
    FadeCtrl(board.LED1),
    FadeCtrl(board.LED2),
    FadeCtrl(board.LED3),
]

# Run animation steps, forever
last_time = time.monotonic()
while True:
    now = time.monotonic()
    time_since = now - last_time
    last_time = now

    for anim in animations:
        anim.step(time_since)
