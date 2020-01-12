#include "led\led.h"
#include "web\web_control.h"

void handleRoot() {
  // String s = MAIN_page; //Read HTML contents
  server.send(200, "text/html", "hello from esp8266!"); //Send web page
}

void setNightMode() {
  FastLED.setBrightness(100);  
  server.send(200, "text/html", "hello from esp8266!");
}

void setDayMode() {
  FastLED.setBrightness(200);  
  server.send(200, "text/html", "hello from esp8266!");
}

void setup() {

  setupLeds();  
  #ifdef TARGET_ESP8266

  // TODO: WiFi-manager
  const char* ssid = "Hamster-wifi";
  const char* password = "134679852";
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    // Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  ArduinoOTA.setHostname("xtree");

  ArduinoOTA.begin();
  // Serial.println("Ready");
  // Serial.print("IP address: ");
  // Serial.println(WiFi.localIP());
  #endif

  
  server.on("/", handleRoot);  
  server.on("/night", setNightMode);
  server.on("/day", setDayMode);
  server.begin();

}


void loop() {
  handleLights();

  #ifdef TARGET_ESP8266
  ArduinoOTA.handle();

  server.handleClient(); 
  #endif
}