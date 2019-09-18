/**
 * Lights up the next LED in sequence each time you press the button.
 * When you turn the board off and on again, the last LED will be remembered and be
 * the starting LED in the sequence.
 */
#include <EEPROM.h>

// Save the LED index to the first byte address in the EEPROM
#define LED_INDEX_ADDR 0

// Maximum index for the LED value
#define LED_MAX 3

uint8_t ledIndex = 0;
uint8_t buttonState = LOW;
uint8_t lastButtonState = LOW;

void setup() {
  pinMode(USER_BTN, INPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);

  // Read LED index from EEPROM
  ledIndex = EEPROM.read(LED_INDEX_ADDR);
  lightLED(ledIndex);
}

void loop() {
  buttonState = digitalRead(USER_BTN);

  // Update the LED index when the button WAS pressed, but is no longer.
  if (!buttonState && lastButtonState) {
    ledIndex++;
    if (ledIndex > LED_MAX) {
      ledIndex = 0;
    }

    // Light up the new LED
    lightLED(ledIndex);

    // Save LED index to EEPROM
    EEPROM.write(LED_INDEX_ADDR, ledIndex);
  }

  lastButtonState = buttonState;
}

/**
 * Light up the LED by index
 */
void lightLED(uint8_t index) {
  // Reset LEDs
  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, LOW);
  digitalWrite(LED_3, LOW);
  digitalWrite(LED_4, LOW);

  // Light the selected LED
  switch(index) {
    case 3:
      digitalWrite(LED_4, HIGH);
      break;
    case 2:
      digitalWrite(LED_3, HIGH);
      break;
    case 1:
      digitalWrite(LED_2, HIGH);
      break;
    default:
      digitalWrite(LED_1, HIGH);
  }
}
