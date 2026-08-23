#include "HX711.h"

const int DT_PIN = 3;
const int SCK_PIN = 2;
const float KNOWN_WEIGHT = 370.0;

HX711 scale;

void setup() {
  Serial.begin(9600);
  scale.begin(DT_PIN, SCK_PIN);
  Serial.println("Taring - hands off");
  delay(5000);
  scale.tare(20);
  Serial.println("Place known weight now - 10 seconds");
  delay(10000);
  long raw = scale.get_value(30);
  Serial.print("Raw: ");
  Serial.println(raw);
  Serial.print("Calibration factor: ");
  Serial.println(raw / KNOWN_WEIGHT, 3);
}

void loop() {
}