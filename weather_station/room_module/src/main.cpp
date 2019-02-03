#include <Arduino.h>
// wifi
#include <WiFiManager.h>
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Needs wifi setup");
}

#include "..\..\common.h"

// blynk
#include <BlynkSimpleEsp8266.h>

void setup() {
  Serial.begin(9600); 

  // wifi connection
  WiFiManager wifiManager;

  //reset settings - for testing
  // wifiManager.resetSettings();

  WiFiManagerParameter custom_blynk_token("blynk-token", "blynk token", blynkToken, 33);
  wifiManager.addParameter(&custom_blynk_token);

    if (!wifiManager.autoConnect("IndoorSensor#1", "134679852")) {
      delay(3000);
      ESP.reset();
      delay(5000);
  }

  Blynk.config(blynkToken);
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(V2, V3, V4);
}

BLYNK_WRITE(V2) {
  Serial.print("V2 ");
  Serial.println(param.asFloat());
}

BLYNK_WRITE(V3) {
  Serial.print("V3 ");
  Serial.println(param.asFloat());
}

BLYNK_WRITE(V4) {
  Serial.print("V4 ");
  Serial.println(param.asFloat());
}

void loop() {
  Blynk.run(); 
}