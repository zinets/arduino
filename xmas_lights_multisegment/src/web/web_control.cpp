#include "web_control.h"
#include "led\led.h"
ESP8266WebServer server(80);

void handleRoot() {
  // String s = MAIN_page; //Read HTML contents
  server.send(200, "text/html", "hello from esp8266!"); //Send web page
}

void sendState() {
    String state = "State is:<br>  night mode is ";
    if (nightMode) {
        state += "On";
    } else {
        state += "Off";
    }

    server.send(200, "text/html", state);
}

void setupWiFi() {
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

    server.on("/", handleRoot);  
    server.on("/night", []() {
        setNightMode(true);
        sendState();
    });
    server.on("/day", []() {
        setNightMode(false);
        sendState();
    });

    server.begin();

}