#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 7
#define DATA_PIN D4
#define FASTLED_ESP8266_D1_PIN_ORDER
#define DELAY 1500

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

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(100);
}



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

void shownum(int number){
  char mask = digits[number];
  for (int x = 0; x < 8; x++) {
    CRGB color = mask >> x & 0x1 == 1 ? colors[number] : CRGB::Black;
    leds[x] = color;
  }  
  FastLED.show();  
}

void loop() {
  for (int i = 0; i < 10; i++) {
    shownum(i);
  
    delay(DELAY);
  }
}