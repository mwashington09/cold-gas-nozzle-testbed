const int PRESSURE_PIN = A0;
const float PSI_MAX = 150.0;

void setup() {
  Serial.begin(9600);
  Serial.println("counts,volts,psi");
}

void loop() {
  int counts = analogRead(PRESSURE_PIN);
  float volts = counts * (5.0 / 1023.0);
  float psi = (volts - 0.5) * (PSI_MAX / 4.0);
  Serial.print(counts);
  Serial.print(",");
  Serial.print(volts, 3);
  Serial.print(",");
  Serial.println(psi, 1);
  delay(200);
}