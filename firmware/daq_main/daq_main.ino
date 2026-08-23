#include "HX711.h"
#include <Adafruit_MAX31855.h>
#include <Adafruit_BME280.h>

const float THRUST_CAL = 48.343;
const float TANK_CAL = 411.030;
const float PSI_MAX = 150.0;
const int BURN_MS = 2000;
const int PRE_MS = 500;
const int POST_MS = 1500;

HX711 thrust;
HX711 tank;
Adafruit_MAX31855 tcPlenum(52, 53, 50);
Adafruit_MAX31855 tcTank(52, 49, 50);
Adafruit_BME280 bme;

float psiZero = 0;
bool bmeOK = false;

void setup() {
  Serial.begin(115200);
  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
  thrust.begin(3, 2);
  thrust.set_scale(THRUST_CAL);
  tank.begin(5, 4);
  tank.set_scale(TANK_CAL);
  tcPlenum.begin();
  tcTank.begin();
  bmeOK = bme.begin(0x76) || bme.begin(0x77);
  Serial.println("# taring - hands off");
  delay(3000);
  thrust.tare(20);
  tank.tare(20);
  long z = 0;
  for (int i = 0; i < 50; i++) { z += analogRead(A0); delay(5); }
  psiZero = z / 50.0;
  Serial.print("# ambient_hPa,");
  Serial.println(bmeOK ? bme.readPressure() / 100.0 : 0.0);
  Serial.print("# ambient_C,");
  Serial.println(bmeOK ? bme.readTemperature() : 0.0);
  Serial.println("# send f to fire");
}

void logRow(unsigned long t0, bool valveOpen) {
  Serial.print(millis() - t0);
  Serial.print(",");
  Serial.print(valveOpen ? 1 : 0);
  Serial.print(",");
  Serial.print(thrust.get_units(1), 2);
  Serial.print(",");
  Serial.print(tank.get_units(1), 2);
  Serial.print(",");
  float v = analogRead(A0) * (5.0 / 1023.0);
  float vz = psiZero * (5.0 / 1023.0);
  Serial.println((v - vz) * (PSI_MAX / 4.0), 2);
}

void loop() {
  if (!Serial.available()) return;
  char c = Serial.read();
  while (Serial.available()) Serial.read();
  if (c != 'f' && c != 'F') return;
  double p0 = tcPlenum.readCelsius();
  double k0 = tcTank.readCelsius();
  Serial.println("ms,valve,thrust_g,tank_g,psi");
  unsigned long t0 = millis();
  while (millis() - t0 < PRE_MS) logRow(t0, false);
  digitalWrite(7, HIGH);
  while (millis() - t0 < PRE_MS + BURN_MS) logRow(t0, true);
  digitalWrite(7, LOW);
  while (millis() - t0 < PRE_MS + BURN_MS + POST_MS) logRow(t0, false);
  Serial.print("# plenum_C_start,");
  Serial.println(p0);
  Serial.print("# tank_C_start,");
  Serial.println(k0);
  Serial.print("# plenum_C_end,");
  Serial.println(tcPlenum.readCelsius());
  Serial.print("# tank_C_end,");
  Serial.println(tcTank.readCelsius());
  Serial.println("# done");
}