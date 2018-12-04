#include "barometer.h"

// i2c
#include <Wire.h>
#include <Adafruit_Sensor.h>    //    id = 31
#include <Adafruit_BMP280.h>    //    id = 528

bool Barometer::update() {
  Adafruit_BMP280 bmp;
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    return false;
  }

  temperature = bmp.readTemperature();
  pressure = bmp.readPressure();

  return true;
}

float Barometer::getPressure () {
  return pressure * 0.75006 / 100;
}

float Barometer::getTemperature () {
  return temperature;
}
