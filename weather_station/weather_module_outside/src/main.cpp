#include <ESP8266WiFi.h>

#include "Barometer.h"

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

    ESP.deepSleep(5 * 60 * 1000000);
}
