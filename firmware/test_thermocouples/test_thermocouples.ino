#include <Adafruit_MAX31855.h>

Adafruit_MAX31855 tc1(52, 53, 50);
Adafruit_MAX31855 tc2(52, 49, 50);

void setup() {
  Serial.begin(9600);
  delay(500);
  tc1.begin();
  tc2.begin();
  Serial.println("plenum_C,tank_C,internal1,internal2");
}

void loop() {
  double a = tc1.readCelsius();
  double b = tc2.readCelsius();
  isnan(a) ? Serial.print("FAULT") : Serial.print(a, 2);
  Serial.print(",");
  isnan(b) ? Serial.print("FAULT") : Serial.print(b, 2);
  Serial.print(",");
  Serial.print(tc1.readInternal(), 2);
  Serial.print(",");
  Serial.println(tc2.readInternal(), 2);
  delay(500);
}