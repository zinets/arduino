#include "web_control.h"
#include "led\led.h"
ESP8266WebServer server(80);

void sendState() {
    String state = "State is:<br>  night mode is ";
    if (nightMode) {
        state += "On (<a href = \"/day\">turn off</a>)";
    } else {
        state += "Off (<a href = \"/night\">turn on</a>)";
    }


    state += "<br>  glitter is ";
    if (isGlitterEnabled) {
        state += "On (<a href = \"/glitter?state=0\">turn off</a>)";
    } else {
        state += "Off (<a href = \"/glitter?state=1\">turn on</a>)";
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

    server.on("/", [](){
        sendState();
    });  
    server.on("/night", []() {
        setNightMode(true);
        sendState();
    });
    server.on("/day", []() {
        setNightMode(false);
        sendState();
    });
    server.on("/glitter", [](){
        setGlitterEnabled(server.arg("state") == "1"); 
        sendState();
    });

    server.begin();

}