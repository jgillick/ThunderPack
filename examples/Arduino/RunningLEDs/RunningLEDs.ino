/*
  Make all the LEDs "run" by turning them on in order, and then off again.
*/

#define SPEED     150    // How fast the LEDs run
#define LED_COUNT 4

int leds[] = {LED_1, LED_2, LED_3, LED_4};
uint8_t ledIdx = 0;
uint8_t mode = HIGH;

void setup() {
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
}

void loop() {
  // Toggle the current LED
  digitalWrite(leds[ledIdx], mode);
  delay(SPEED);

  // Increment LED
  ledIdx++;
  if (ledIdx >= LED_COUNT) {
    ledIdx = 0;
    if (mode == HIGH) {
      mode = LOW;
    } else {
      mode = HIGH;
    }
  }
}
