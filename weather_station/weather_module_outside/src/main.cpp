#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>    //    id = 31
#include <Adafruit_BMP280.h>    //    id = 528

Adafruit_BMP280 bmp; // I2C

void setup() {

  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
}

void loop() {
    Serial.print("Temperature = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    delay(2000);
}
