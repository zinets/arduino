#include <Arduino.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bme;

#include <WiFiManager.h>

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Needs wifi setup");
}

#include <BlynkSimpleEsp8266.h>
char  blynkToken[33] = "48ed6d953c2e4a5f8ff06fe5f2a8b415";

#define VIRTUAL_SENSOR_BALKONY_TEMPERATURE  V0
#define VIRTUAL_SENSOR_BALKONY_PRESSURE     V1

#define DEBUG
#define DEEP_SLEEP
#define DEEP_SLEEP_TIMEOUT 15

void setup() {
  #ifdef DEBUG
  Serial.begin(9600);
  #endif
    
  if (!bme.begin()) {  
    #ifdef DEBUG
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    #endif
    while (1);
  }

  WiFiManager wifiManager;
  wifiManager.setBreakAfterConfig(true);
  // wifiManager.setAPCallback(configModeCallback);

  WiFiManagerParameter custom_blynk_text("<br/>Balcony  config. <br/>");
  wifiManager.addParameter(&custom_blynk_text);
  WiFiManagerParameter custom_blynk_token("blynk-token", "blynk token", blynkToken, 33);
  wifiManager.addParameter(&custom_blynk_token);

  if (!wifiManager.autoConnect()) {
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  Blynk.config(blynkToken);

}

void loop() {
  float temp = bme.readTemperature();
  float pressure = bme.readPressure();
  
  #ifdef DEBUG
  Serial.print("Temperature = ");    
  Serial.print(temp);
  Serial.println(" *C");
  
  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" Pa");

  Serial.println();
  #endif

  Blynk.virtualWrite(V0, VIRTUAL_SENSOR_BALKONY_TEMPERATURE);
  Blynk.virtualWrite(V1, VIRTUAL_SENSOR_BALKONY_PRESSURE);
  Blynk.run();

  #ifdef DEEP_SLEEP
  ESP.deepSleep(DEEP_SLEEP_TIMEOUT * 60 * 1000000);
  #else
  delay(5 * 1000);
  #endif
}

