#include "digits.h"

CRGB leds[NUM_LEDS];

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

const CRGB onDot = CRGB::Fuchsia;
const CRGB updatingDot = CRGB::Green;
const CRGB errorDot = CRGB::Red;

void setupLed() {
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(200);
}

void showDigit(int startIndex, int number) {
  char mask = digits[number];
  for (int x = 0; x < 7; x++) {
    CRGB color = (mask >> x & 0x1) == 1 ? CRGB::Fuchsia : CRGB::Black;
    leds[startIndex + x] = color;
  }     
}

void showDots(int startIndex, bool visible) {
  CRGB color = visible ? onDot : CRGB::Black;
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

void showStartScreen() {
  for (int x = 0; x < NUM_LEDS; x++) {
    leds[x] = CRGB::Black;    
  }
  int onIndexes[] = {0, 7, 16, 23, 24, 25, 26, 19, 10, 3, 4, 5};
  for (int x = 0; x < 12; x++) {
    leds[onIndexes[x]] = CRGB::Green;
  }
}

void showErrorScreen() {
  for (int x = 0; x < NUM_LEDS; x++) {
    leds[x] = CRGB::Black;    
  }
  int onIndexes[] = {6, 13, 22, 29};
  for (int x = 0; x < 4; x++) {
    leds[onIndexes[x]] = errorDot;
  }
}