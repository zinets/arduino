#include <Arduino.h>
// wifi
#include <WiFiManager.h>
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Needs wifi setup");
}

// blynk
#include <BlynkSimpleEsp8266.h>
char  blynkToken[33] = "ebd8b40ad2104b7cb5165ffc5cc66781";

#define VIRTUAL_SENSOR_BALKONY_TEMPERATURE  V0
#define VIRTUAL_SENSOR_BALKONY_PRESSURE     V1

#define VIRTUAL_SENSOR_OUTDOOR_TEMPERATURE  V2
#define VIRTUAL_SENSOR_OUTDOOR_PRESSURE     V3
#define VIRTUAL_SENSOR_OUTDOOR_HUMIDITY     V4
#define VIRTUAL_SENSOR_OUTDOOR_VOLTAGE      V5

// sensor
#define READ_HUMIDITY

#include <Adafruit_Sensor.h>
#ifdef READ_HUMIDITY
#include <Adafruit_BME280.h>
Adafruit_BME280 bme;
#else
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bme;
#endif

#undef DEBUG
#define DEEP_SLEEP
#define DEEP_SLEEP_TIMEOUT 15

void setup() {
  #ifdef DEBUG
  Serial.begin(9600);
  #endif


  if (!bme.begin(0x76)) {  
    #ifdef DEEP_SLEEP
    #ifdef DEBUG
    Serial.println("Go to sleep because sensor hardware not found");
    #endif
    ESP.deepSleep(DEEP_SLEEP_TIMEOUT * 60 * 1000000);
    #endif
  }

  WiFiManager wifiManager;

  //reset settings - for testing
  // wifiManager.resetSettings();

  #ifndef DEBUG
  wifiManager.setDebugOutput(false);
  #endif
  
  wifiManager.setBreakAfterConfig(true);
  wifiManager.setConfigPortalTimeout(60);
  // wifiManager.setAPCallback(configModeCallback);

  WiFiManagerParameter custom_blynk_text("<br/>Outdoor sensor config. <br/>");
  wifiManager.addParameter(&custom_blynk_text);
  WiFiManagerParameter custom_blynk_token("blynk-token", "blynk token", blynkToken, 33);
  wifiManager.addParameter(&custom_blynk_token);

  if (!wifiManager.autoConnect("OutdoorSensor#1", "134679852")) {
    #ifdef DEBUG
    Serial.println("Go to sleep because no wifi access");
    #endif
    ESP.deepSleep(DEEP_SLEEP_TIMEOUT * 60 * 1000000);

    // delay(3000);
    // ESP.reset();
    // delay(5000);
  }

  Blynk.config(blynkToken);
}

void loop() {

  float temp = bme.readTemperature();
  float pressure = bme.readPressure() / 133.322;
  float humidity = bme.readHumidity();
  float v = analogRead(A0) * 0.00504;
  
  #ifdef DEBUG
  Serial.print("Temperature = ");    
  Serial.print(temp);
  Serial.println(" *C");
  
  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" mm");

  Serial.print("Humidity = ");
  Serial.print(humidity);
  Serial.println(" %%");

  Serial.print("Voltage = ");
  Serial.print(v);
  Serial.println(" V");

  Serial.println();
  #endif

  while (!Blynk.connect()) {

  }
  #ifdef DEBUG
  Serial.println("Connected");
  #endif

  Blynk.virtualWrite(VIRTUAL_SENSOR_OUTDOOR_TEMPERATURE, temp);
  Blynk.virtualWrite(VIRTUAL_SENSOR_OUTDOOR_PRESSURE, pressure);
  Blynk.virtualWrite(VIRTUAL_SENSOR_OUTDOOR_HUMIDITY, humidity);
  Blynk.virtualWrite(VIRTUAL_SENSOR_OUTDOOR_VOLTAGE, v);

  Blynk.run();

  delay(1000);

  #ifdef DEEP_SLEEP
  #ifdef DEBUG
  Serial.println("Go to sleep..");
  #endif
  ESP.deepSleep(DEEP_SLEEP_TIMEOUT * 60 * 1000000);
  // ESP.deepSleep(30  * 1000000);
  #else
  delay(5 * 1000);
  #endif
}

