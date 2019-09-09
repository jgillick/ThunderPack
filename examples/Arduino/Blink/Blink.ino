/*
  Turns an LED on for one second, then off for one second, repeatedly.
*/

void setup() {
  pinMode(LED_1, OUTPUT);
}

void loop() {
  digitalWrite(LED_1, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_1, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
