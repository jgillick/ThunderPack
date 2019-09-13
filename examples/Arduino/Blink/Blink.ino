/*
  Turns an LED on for one second, then off for one second, repeatedly.
  Pressing the button will make the LED blink faster.
*/

#define SPEED_SLOW 800
#define SPEED_FAST 200

int blinkSpeed = SPEED_SLOW;
uint8_t ledState = LOW;

void setup() {
  pinMode(LED_1, OUTPUT);
  pinMode(USER_BTN, INPUT);
}

void loop() {
  // Toggle the LED state
  if (ledState == LOW) {
    digitalWrite(LED_1, HIGH);
    ledState = HIGH;
  } else {
    digitalWrite(LED_1, LOW);
    ledState = LOW;
  }

  // Blink faster while the button has been pressed
  if (digitalRead(USER_BTN) == HIGH) {
    blinkSpeed = SPEED_FAST;
  } else {
    blinkSpeed = SPEED_SLOW;
  }
  delay(blinkSpeed);
}
