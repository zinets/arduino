#include <Arduino.h>

#include <Adafruit_SH1106.h>
#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);

void setup() {
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.setRotation(2);
  display.clearDisplay();
}

void loop() {
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(11,25);
  display.println("Joy Sense 2");
  display.display();
}