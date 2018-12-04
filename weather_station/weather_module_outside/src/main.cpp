#include <Arduino.h>
#include <ESP8266WiFi.h>

// i2c
#include <Wire.h>
#include <Adafruit_Sensor.h>    //    id = 31
#include <Adafruit_BMP280.h>    //    id = 528

Adafruit_BMP280 bmp; // I2C

// blynk
#include <BlynkSimpleEsp8266.h>

char auth[] = "48ed6d953c2e4a5f8ff06fe5f2a8b415";
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";


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
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
}

void loop() {
    float f = bmp.readTemperature();
    Serial.print("Temperature = ");
    Serial.print(f);
    Serial.println(" *C");

    Blynk.virtualWrite(0, f);

    f = bmp.readPressure();
    Serial.print("Pressure = ");
    Serial.print(f);
    Serial.println(" Pa");

    Blynk.virtualWrite(1, f);

    ESP.deepSleep(5 * 60 * 1000000); 
}
