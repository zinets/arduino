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

#include <Adafruit_BMP280.h>
Adafruit_BMP280 bme;

#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"
/*

LCD - Wemos
LED - 3.3v
SCK - D5
SDA - D7
A0  - D3 
RST - D4 - можно подключить к RST контроллера и сэкономить одну ножку
CS  - D8
GND - GND
VCC - 5v

*/
#define TFT_CS     D8
// #define TFT_RST    D4
#define TFT_RST    0
#define TFT_DC     D3

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
void testdrawtext(char *text, uint16_t color, int yPos) {
  tft.setCursor(0, yPos);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.setTextSize(2);
  tft.print(text);
}

void setup() {
  Serial.begin(9600); 

  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST77XX_BLACK);

  testdrawtext("Lorem", ST77XX_WHITE, 0);
  testdrawtext("123", ST77XX_RED, 16);
  testdrawtext("asЙцУ00", ST77XX_YELLOW, 32);

  if (!bme.begin()) {
    Serial.println("BMP280 not found!");
    testdrawtext("BMP280!!!", ST7735_MAGENTA, 40);
  }

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
  float intTemp = bme.readTemperature();
  Serial.print("temperature:");
  Serial.println(intTemp);

  Blynk.run(); 

  // TO DO: обновление через DEEP_SLEEP_TIMEOUT минут данных внешнего датчика
}