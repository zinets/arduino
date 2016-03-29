/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;
StaticJsonBuffer<1024 * 10> jsonBuffer;

void setup() {

  USE_SERIAL.begin(115200);
  // USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  WiFiMulti.addAP("hamster-wifi2", "134679852");
}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    HTTPClient http;

    USE_SERIAL.print("[HTTP] begin...\n");
    http.begin("api.wunderground.com", 80, "/api/d85b09d20c0e17b4/conditions/q/UA/Zaporizhzhya.json"); //HTTP

    USE_SERIAL.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    if (httpCode) {
      // HTTP header has been send and Server response header has been handled
      USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == 200) {
        String payload = http.getString();
        USE_SERIAL.println(payload);

        JsonObject& root = jsonBuffer.parseObject(payload);
        if (!root.success()) {
          USE_SERIAL.println("parseObject() failed");
          while (1);
        } else {
          const char* res = root["current_observation"]["temperature_string"];
          USE_SERIAL.println(res);
          res = root["current_observation"]["relative_humidity"];
          USE_SERIAL.println(res);
          res = root["current_observation"]["feelslike_string"];
          USE_SERIAL.println(res);
        }
      }
    } else {
      USE_SERIAL.print("[HTTP] GET... failed, no connection or no HTTP server\n");
    }
    delay(20000);
  } else {
    USE_SERIAL.print("waiting...\n");
    delay(5000);
  }
}

