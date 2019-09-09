/*
  Fade the first LED on and off
*/

#define INCREMENT 5 // How much the brightness changes each cycle
#define SPEED 30    // The ms delay between cycles.

int fadeVal = 0;    // how bright the LED is
int direction = 1;  // The direction we're incrementing the brightness (1 or -1)

void setup() {
  pinMode(LED_1, OUTPUT);
}

void loop() {
  // Set the LED brithness value
  analogWrite(LED_1, fadeVal);

  // Increment and reverse if we've reached the limits
  fadeVal += INCREMENT * direction;
  if (fadeVal <= 0 || fadeVal >= 255) {
    direction *= -1;
  }

  delay(SPEED);
}
