#include <ESP8266WiFi.h>

#include "barometer.h"
#include "dallas_temp.h"

// blynk
#include <BlynkSimpleEsp8266.h>

char auth[] = "48ed6d953c2e4a5f8ff06fe5f2a8b415";
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

#define DALLAS_SENSOR_PIN 2

/*

- добавить глубокий сон
- добавить мониторинг напряжения
- добавить WiFiManager - настройка подключения к wifi
  - название точки доступа при настройке + возможность подключения к ней с паролем?
    ИЛИ
  - не париться от слова "вообще" и зашить параметры доступа намертво? или хотя бы для начала
- добавить Blynk - отправку на сервер текущих значений
- добавить чтение температуры из 1-wire


*/

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
}

void loop() {
    Barometer baro;
    if (baro.update()) {
      float f = baro.getTemperature();
      Serial.print("Temperature = ");
      Serial.print(f);
      Serial.println(" *C");

      Blynk.virtualWrite(0, f);

      f = baro.getPressure();
      Serial.print("Pressure = ");
      Serial.print(f);
      Serial.println(" Pa");

      Blynk.virtualWrite(1, f);
    }

#ifdef DALLAS_SENSOR_PIN
    DallasSensor ds(DALLAS_SENSOR_PIN);
    if (ds.update()) {
      float f = ds.getTemperature();
      Serial.print("Temperature (1-wire) = ");
      Serial.print(f);
      Serial.println(" *C");

      Blynk.virtualWrite(2, f);
    }

  #endif
    int sensorValue = analogRead(A0);
    float voltage = sensorValue * (5.0 / 1023.0);

    ESP.deepSleep(5 * 60 * 1000000);
}
