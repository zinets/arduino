#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>    //    id = 31
#include <Adafruit_BMP280.h>    //    id = 528

Adafruit_BMP280 bmp; // I2C

#define USE_BLYNK
#undef USE_WIFI_MANAGER

#ifdef USE_BLYNK
#include  <BlynkSimpleEsp8266.h > //  id = 415
#endif



/*

- добавить глубокий сон
- добавить мониторинг напряжения
- добавить WiFiManager - настройка подключения к wifi
  - название точки доступа при настройке + возможность подключения к ней с паролем?
    ИЛИ
  - не париться от слова "вообще" и зашить параметры доступа намертво? или хотя бы для начала
- добавить Blynk - отправку на сервер текущих значений



*/

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
