#include <Arduino.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bme;

#include <WiFiManager.h>
char  blynkToken[33] = "48ed6d953c2e4a5f8ff06fe5f2a8b415";


void configModeCallback (WiFiManager *myWiFiManager);

void setup() {
  Serial.begin(9600);
  Serial.println(F("BMP280 test"));
  
  if (!bme.begin()) {  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }

  WiFiManager wifiManager;
  wifiManager.setBreakAfterConfig(true);
  wifiManager.setAPCallback(configModeCallback);

  WiFiManagerParameter custom_blynk_text("<br/>Balcony  config. <br/>");
  wifiManager.addParameter(&custom_blynk_text);
  WiFiManagerParameter custom_blynk_token("blynk-token", "blynk token", blynkToken, 33);
  wifiManager.addParameter(&custom_blynk_token);

  if (!wifiManager.autoConnect()) {
    delay(3000);
    ESP.reset();
    delay(5000);
  }
}

void loop() {
  Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");
    
    Serial.print("Pressure = ");
    Serial.print(bme.readPressure());
    Serial.println(" Pa");

    Serial.print("Approx altitude = ");
    Serial.print(bme.readAltitude(1013.25)); // this should be adjusted to your local forcase
    Serial.println(" m");
    
    Serial.println();
    delay(2000);
}


void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Needs wifi setup");
}