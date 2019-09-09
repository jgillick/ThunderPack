/*
  Turns on the first LED when the user button is pressed.
*/

int buttonState = 0;

void setup() {
  pinMode(LED_1, OUTPUT);
  pinMode(USER_BTN, INPUT);
}

void loop() {
  // If the button has been pressed, turn the LED on
  if (digitalRead(USER_BTN) == HIGH) {
    digitalWrite(LED_1, HIGH);
  } else {
    digitalWrite(LED_1, LOW);
  }
}
