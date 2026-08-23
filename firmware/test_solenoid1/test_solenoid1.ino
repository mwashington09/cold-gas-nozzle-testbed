const int SOLENOID_PIN = 7;

void setup() {
  pinMode(SOLENOID_PIN, OUTPUT);
  digitalWrite(SOLENOID_PIN, LOW);
}

void loop() {
  digitalWrite(SOLENOID_PIN, HIGH);
  delay(1000);
  digitalWrite(SOLENOID_PIN, LOW);
  delay(1000);
}
