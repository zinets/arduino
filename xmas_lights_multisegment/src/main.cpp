// MultipleStripsInOneArray - see https://github.com/FastLED/FastLED/wiki/Multiple-Controller-Examples for more info on
// using multiple controllers.  In this example, we're going to set up four NEOPIXEL strips on three
// different pins, each strip will be referring to a different part of the single led array

#include <FastLED.h>

#define NUM_STRIPS 5
#define START_PIN 2
#define NUM_LEDS_PER_STRIP 17
#define NUM_LEDS NUM_LEDS_PER_STRIP * NUM_STRIPS

CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];

void setup() {
  FastLED.addLeds<NEOPIXEL, 2>(leds, 0 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 3>(leds, 1 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 4>(leds, 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 5>(leds, 3 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 6>(leds, 4 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);

}

void loop() {
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Blue;
    FastLED.show();
    // leds[i] = CRGB::Black;
    delay(100);
  }
}