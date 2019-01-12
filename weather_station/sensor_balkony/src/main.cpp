#include <Arduino.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bme;

#include <WiFiManager.h>

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Needs wifi setup");
}

#include <BlynkSimpleEsp8266.h>
char  blynkToken[33] = "ebd8b40ad2104b7cb5165ffc5cc66781";

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
  float pressure = bme.readPressure() / 133.322;
  
  #ifdef DEBUG
  Serial.print("Temperature = ");    
  Serial.print(temp);
  Serial.println(" *C");
  
  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" mm");

  Serial.println();
  #endif

  while (!Blynk.connect()) {

  }
  #ifdef DEBUG
  Serial.println("Connected");
  #endif

  Blynk.virtualWrite(VIRTUAL_SENSOR_BALKONY_TEMPERATURE, temp);
  Blynk.virtualWrite(VIRTUAL_SENSOR_BALKONY_PRESSURE, pressure);
  Blynk.run();

  delay(1000);

  #ifdef DEEP_SLEEP
  #ifdef DEBUG
  Serial.println("Go to sleep..");
  #endif
  // ESP.deepSleep(DEEP_SLEEP_TIMEOUT * 60 * 1000000);
  ESP.deepSleep(30  * 1000000);
  #else
  delay(5 * 1000);
  #endif
}

