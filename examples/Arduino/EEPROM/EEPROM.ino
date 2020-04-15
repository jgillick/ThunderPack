/**
 * A simple example to demonstrate storing a value in non-volatile memory.
 * This program will light up the next LED in sequence each time you press the button.
 * When you turn the board off and on again, the last LED will be remembered and be
 * the starting LED in the sequence.
 */
#include <EEPROM.h>

// Save the LED index to the first byte address in the EEPROM
#define LED_SAVE_ADDRESS  0

// Number of LEDs
#define LED_COUNT 4

int leds[] = {LED_1, LED_2, LED_3, LED_4};
uint8_t ledIndex = 0;
uint8_t buttonState;
uint8_t lastButtonState = LOW;

void setup() {
  pinMode(USER_BTN, INPUT);
  for (int i = 0; i < LED_COUNT; i++) {
    pinMode(leds[i], OUTPUT);
  }

  // Read LED index from EEPROM
  EEPROM.get(LED_SAVE_ADDRESS, ledIndex);
  if (ledIndex > LED_COUNT) {
    ledIndex = 0;
  }
  lightLED(ledIndex);
}

void loop() {
  buttonState = digitalRead(USER_BTN);

  // Update the LED index when the button state changes
  if (buttonState && !lastButtonState) {
    ledIndex++;
    if (ledIndex >= LED_COUNT) {
      ledIndex = 0;
    }

    // Light up the new LED
    lightLED(ledIndex);

    // Save LED index to EEPROM
    EEPROM.update(LED_SAVE_ADDRESS, ledIndex);
  }

  lastButtonState = buttonState;
  delay(10);
}

/**
 * Light up the LED by index
 */
void lightLED(uint8_t index) {
  for (int i = 0; i < LED_COUNT; i++) {
    if (i == index) {
      digitalWrite(leds[i], HIGH);
    } else {
      digitalWrite(leds[i], LOW);
    }
  }
}
