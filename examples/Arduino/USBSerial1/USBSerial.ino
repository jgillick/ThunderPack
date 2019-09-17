/**
 * Outputs the current button state to the serial monitor.
 */
 
void setup() {
  Serial.begin(9600);
  pinMode(USER_BTN, INPUT);
}

void loop() {
  int buttonState = digitalRead(USER_BTN);
  Serial.println(buttonState);
  delay(100);
}
