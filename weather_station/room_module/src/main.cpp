#include <Arduino.h>
// wifi
#include <WiFiManager.h>
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Needs wifi setup");
}

#include "..\..\common.h"

// blynk
#include <BlynkSimpleEsp8266.h>

// outdoor data
float outdoor_temperature = 0;
float outdoor_pressure = 0;
float outdoor_humidity = 0;

BLYNK_CONNECTED() {
  Blynk.syncVirtual(VIRTUAL_SENSOR_OUTDOOR_HUMIDITY, 
    VIRTUAL_SENSOR_OUTDOOR_HUMIDITY, 
    VIRTUAL_SENSOR_OUTDOOR_HUMIDITY);
}

BLYNK_WRITE(VIRTUAL_SENSOR_OUTDOOR_TEMPERATURE) {
  outdoor_temperature = param.asFloat();
  Serial.print("V2 ");
  Serial.println(outdoor_temperature);  
}

BLYNK_WRITE(VIRTUAL_SENSOR_OUTDOOR_PRESSURE) {
  outdoor_pressure = param.asFloat();
  Serial.print("V3 ");
  Serial.println(outdoor_pressure);
}

BLYNK_WRITE(VIRTUAL_SENSOR_OUTDOOR_HUMIDITY) {
  outdoor_humidity = param.asFloat();
  Serial.print("V4 ");
  Serial.println(outdoor_humidity);
}

#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"
#define TFT_CS     D8
#define TFT_RST    D4
#define TFT_DC     D3

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.setTextSize(2);
  tft.print(text);
}

void setup() {
  Serial.begin(9600); 

  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST77XX_BLACK);

  testdrawtext("Lorem ipsum dolor sit suada nunc bibendum. Nullam aliquet ultrices m ", ST77XX_WHITE);
  
  return;
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

void loop() {
  Blynk.run(); 

  // TO DO: обновление через DEEP_SLEEP_TIMEOUT минут данных внешнего датчика
}