#include "led\led.h"
#include "web\web_control.h"

void setup() {
  setupLeds();  
  #ifdef TARGET_ESP8266
  setupWiFi();
  #endif  
}

void loop() {
  handleLights();

  #ifdef TARGET_ESP8266
  ArduinoOTA.handle();
  server.handleClient(); 
  #endif
}