#include <FastLED.h>

#define NUM_STRIPS 6
#define START_PIN 2
#define NUM_LEDS_PER_STRIP 17
#define NUM_LEDS NUM_LEDS_PER_STRIP * NUM_STRIPS
#define FRAMES_PER_SECOND  120
// скорость изменения цвета
#define COLOR_CHANGE_SPEED 50 
// частота вспышек
#define GLITTER_FREQUENCY 200
// яркость вспышек
#define GLITTER_INTENSITY 65

CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];
uint8_t gHue = 0; 

void setup() {
  FastLED.addLeds<NEOPIXEL, 2>(leds, 0 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 3>(leds, 1 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 4>(leds, 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 5>(leds, 3 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 6>(leds, 4 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 7>(leds, 5 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);

  FastLED.setBrightness(100); // 0 - 255
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void rainbow() {
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() {
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  EVERY_N_MILLISECONDS( GLITTER_FREQUENCY ) { addGlitter(GLITTER_INTENSITY); } 
  
}

void loop() {
  rainbowWithGlitter();
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( COLOR_CHANGE_SPEED ) { gHue++; } // slowly cycle the "base color" through the rainbow
}