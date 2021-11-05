#include "digits.h"


CRGB leds[NUM_LEDS];
CRGB colors[10] = {
  CRGB::SandyBrown,
  CRGB::Sienna,
  CRGB::Salmon,
  CRGB::SandyBrown,
  CRGB::SeaGreen,
  CRGB::Seashell,
  CRGB::Sienna,
  CRGB::Silver,
  CRGB::SkyBlue,
  CRGB::SlateBlue,
};

char digits[10] = {
  0b0111111, 
  0b0000110,
  0b1011011,
  0b1001111,
  0b1100110,
  0b1101101,
  0b1111101,
  0b0000111,
  0b1111111,
  0b1101111,
  };

void setupLed() {
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(100);
}

void showDigit(int startIndex, int number) {
  char mask = digits[number];
  for (int x = 0; x < 7; x++) {
    CRGB color = (mask >> x & 0x1) == 1 ? colors[number] : CRGB::Black;
    leds[startIndex + x] = color;
  }     
}

void showDots(int startIndex, bool visible) {
  CRGB color = visible ? CRGB::Indigo : CRGB::Black;
  leds[startIndex] = color;
  leds[startIndex + 1] = color;
}

void displayTime(DateTime time) {

  int d = time.hour() / 10;
  showDigit(0, d);
  d = time.hour() % 10;
  showDigit(7, d);
  
  d = time.second() % 2;
  showDots(14, d == 0);

  d = time.minute() / 10;
  showDigit(16, d);
  d = time.minute() % 10;
  showDigit(23, d);
  
  FastLED.show(); 
}