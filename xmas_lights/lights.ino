#include "FastLED.h"

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if FASTLED_VERSION < 3001000
//#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    6
//#define CLK_PIN   4
#define LED_TYPE    NEOPIXEL
#define COLOR_ORDER GRB
#define NUM_LEDS    149
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          200
#define FRAMES_PER_SECOND  8

void setup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
// Each pattern is defined as a function that takes two uint8_t's; all of the pattern functions
// have to have the same signature in this setup, and two (hardcoded!) parameters can be 
// passed to each one:

typedef void (*TwoArgumentPattern)(uint8_t arg1, uint8_t arg2);
typedef struct { 
  TwoArgumentPattern mPattern;
  uint8_t mArg1;
  uint8_t mArg2;
} TwoArgumentPatterWithArgumentValues;

TwoArgumentPatterWithArgumentValues gPatternsAndArguments[] = {
  {rainbowWithGlitter_2,  5 /*stripeDensity*/,  0 /*chanceOfGlitter*/},
  {rainbowWithGlitter_2, 10 /*stripeDensity*/, 80 /*chanceOfGlitter*/},

  // {sinelon_2, 13 /*BPM*/, 10 /*fadeAmount*/ },
  // {sinelon_2,  7 BPM, 1 /*fadeAmount*/ },
  
  // {bpm_2,     62 /*BPM*/, 3 /*stripeWidth*/},
  // {bpm_2,    125 /*BPM*/, 7 /*stripeWidth*/},
  // {bpm_2,     15 /*BPM*/, 1 /*stripeWidth*/}, // ! подобрать ширину
  
  // {confetti_2, 96 /*colorVariation*/, 30/*fadeAmount*/},
  // {confetti_2, 16 /*colorVariation*/,  3/*fadeAmount*/},
  
  // {juggle_2,  3 /*numDots*/,  7 /*baseBpmSpeed*/},
  // {juggle_2,  8 /*numDots*/, 13 /*baseBpmSpeed*/}
};


uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  
void loop()
{
  gHue++; // slowly cycle the "base color" through the rainbow

  // Call the current pattern function once, updating the 'leds' array
  uint8_t arg1 = gPatternsAndArguments[ gCurrentPatternNumber ].mArg1;
  uint8_t arg2 = gPatternsAndArguments[ gCurrentPatternNumber ].mArg2;
  TwoArgumentPattern pat = gPatternsAndArguments[ gCurrentPatternNumber ].mPattern;
  
  pat(arg1, arg2);
  
  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); // about sixty FPS

  EVERY_N_SECONDS(10) { nextPattern(); } // change patterns periodically
}

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  const int numberOfPatterns = sizeof(gPatternsAndArguments) / sizeof( gPatternsAndArguments[0]);  
  gCurrentPatternNumber = (gCurrentPatternNumber+1) % numberOfPatterns;
}


void rainbowWithGlitter_2( uint8_t stripeDensity, uint8_t chanceOfGlitter) 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  fill_rainbow( leds, NUM_LEDS, gHue, stripeDensity);
  addGlitter(chanceOfGlitter);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti_2( uint8_t colorVariation, uint8_t fadeAmount)
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, fadeAmount);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(colorVariation), 200, 255);
}

void sinelon_2( uint8_t bpmSpeed, uint8_t fadeAmount)
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, fadeAmount);
  int pos = beatsin16(bpmSpeed, 0, NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm_2( uint8_t bpmSpeed, uint8_t stripeWidth)
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = bpmSpeed;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(palette, gHue+(i*stripeWidth), beat);
  }
}

void juggle_2( uint8_t numDots, uint8_t baseBpmSpeed) {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 100);
  byte dothue = 0;
  for( int i = 0; i < numDots; i++) {
    leds[beatsin16(i+baseBpmSpeed,0,NUM_LEDS)] |= CHSV(dothue, 255, 224);
    dothue += (256 / numDots);
  }
}
