const int PRESSURE_PIN = A0;
const int SOLENOID_PIN = 7;
const float PSI_MAX = 150.0;

void setup() {
  Serial.begin(9600);
  pinMode(SOLENOID_PIN, OUTPUT);
  digitalWrite(SOLENOID_PIN, LOW);
  Serial.println("Send o to open, c to close");
  Serial.println("psi");
}

void loop() {
  if (Serial.available()) {
    char k = Serial.read();
    if (k == 'o') digitalWrite(SOLENOID_PIN, HIGH);
    if (k == 'c') digitalWrite(SOLENOID_PIN, LOW);
  }
  float v = analogRead(PRESSURE_PIN) * (5.0 / 1023.0);
  Serial.println((v - 0.489) * (PSI_MAX / 4.0), 2);
  delay(100);
}