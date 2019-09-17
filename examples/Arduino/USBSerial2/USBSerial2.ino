/**
 * Echos any character you send the device on the serial monitor
 */

// Character codes
#define NL_CODE 10
#define CR_CODE 13

int readByte = 0;

void setup() {
  Serial.begin(9600);
  pinMode(USER_BTN, INPUT);
  pinMode(LED_1, OUTPUT);
  digitalWrite(LED_1, HIGH);
}

void loop() {
  if (Serial.available() > 0) {
    readByte = Serial.read();

    // Ignore the following character codes
    if (readByte == NL_CODE || readByte == CR_CODE) {
      return;
    }
    
    Serial.print("You typed: \"");
    Serial.print((char)readByte);
    Serial.println("\"");
  }
}
