#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include "Adafruit_I2CDevice.h"

#define cs   D8
#define dc   D3
#define rst  D4

Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);

void displayTemperature(float main_temp);

void loop() {
  displayTemperature(37.3);
  delay(1500);
}

#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0
#define WHITE    0xFFFF
#define GREY     0xC618

void setup(void) {
   Serial.begin(9600);

  Wire.begin();

  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  tft.fillScreen(BLACK);
  tft.setCursor(30, 80);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.fillScreen(BLACK);
  tft.setCursor(30, 80);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.print("Ready");
}

void displayTemperature(float main_temp) {
  tft.setTextColor(WHITE);
  tft.setTextSize(3);

  tft.setCursor(5, 20);
  String temperatureValue = String(main_temp) + (char)247 + "C";
  tft.print(temperatureValue);
}