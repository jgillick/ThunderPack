/**
 * Outputs the current button state to the serial monitor, each time it changes.
 * If you enter a number from 1 - 4, that LED will toggle on or off.
 * Be sure to select "CDC" from the Tools > USB Support menu.
 */

int btnState = 0;
int lastBtnState = 0;
int readByte = 0;
 
void setup() {
  // Setup serial
  Serial.begin(9600);

  // Setup pins
  pinMode(USER_BTN, INPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
}

void loop() {
  printButtonChange();
  readSerialInput();
  delay(100);
}

void printButtonChange() {
btnState = digitalRead(USER_BTN);
  if (btnState != lastBtnState) {
    Serial.println(btnState);
  }
  lastBtnState = btnState;
}

void readSerialInput() {
  if (Serial.available() > 0) {    
    readByte = Serial.read();
    switch (readByte) {
      case '1':
        digitalWrite(LED_1, !digitalRead(LED_1));
        Serial.println("Toggle LED 1");
        break;
      case '2':
        digitalWrite(LED_2, !digitalRead(LED_2));
        Serial.println("Toggle LED 2");
        break;
      case '3':
        digitalWrite(LED_3, !digitalRead(LED_3));
        Serial.println("Toggle LED 3");
        break;
      case '4':
        digitalWrite(LED_4, !digitalRead(LED_4));
        Serial.println("Toggle LED 4");
        break;
    }
  }
}
