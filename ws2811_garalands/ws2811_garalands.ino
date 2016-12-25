#include "FastLED.h"



#define NUM_LEDS     69
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define DATA_PIN      2

#undef CONTROLLER
#ifdef CONTROLLER
#define MODE_PIN      13
#endif

CRGB leds[NUM_LEDS];

#define TWINKLE_SPEED         3
#define TWINKLE_DENSITY       5
#define SECONDS_PER_PALETTE   60

CRGB gBackgroundColor = CRGB::Black; //CRGB(0,0,6);
CRGBPalette16 gCurrentPalette;
CRGBPalette16 gTargetPalette;

#ifdef CONTROLLER
#define MODE_DAY    1
#define MODE_NIGHT  0
boolean curMode = MODE_DAY;
#endif

void setup() {
  delay( 3000 ); //safety startup delay

#ifdef CONTROLLER
  pinMode(MODE_PIN, INPUT);
  digitalWrite(MODE_PIN, HIGH);
#endif
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
  .setCorrection(TypicalLEDStrip);

  chooseNextColorPalette(gTargetPalette);
}


void loop() {
  EVERY_N_SECONDS( SECONDS_PER_PALETTE ) {
    chooseNextColorPalette( gTargetPalette );
  }

  EVERY_N_MILLISECONDS( 10 ) {
    nblendPaletteTowardPalette( gCurrentPalette, gTargetPalette, 12);
  }

  drawTwinkles( leds, NUM_LEDS);

  FastLED.show();

#ifdef CONTROLLER
  boolean mode = digitalRead(MODE_PIN);
  if (mode != curMode) {
    curMode = mode;
    if (curMode == MODE_DAY) {
      FastLED.setBrightness(200);
    } else {
      FastLED.setBrightness(100);
    }
  }
#endif
}


//  This function loops over each pixel, calculates the
//  adjusted 'clock' that this pixel should use, and calls
//  "CalculateOneTwinkle" on each pixel.  It then displays
//  either the twinkle color of the background color,
//  whichever is brighter.
void drawTwinkles( CRGB* L, uint16_t N)
{
  // "PRNG16" is the pseudorandom number generator
  // It MUST be reset to the same starting value each time
  // this function is called, so that the sequence of 'random'
  // numbers that it generates is (paradoxically) stable.
  uint16_t PRNG16 = 11337;

  uint32_t clock32 = millis();

  uint8_t backgroundBrightness = gBackgroundColor.getAverageLight();

  for ( uint16_t i = 0; i < N; i++) {
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    uint16_t myclockoffset16 = PRNG16; // use that number as clock offset
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    // use that number as clock speed adjustment factor (in 8ths, from 8/8ths to 23/8ths)
    uint8_t myspeedmultiplierQ5_3 =  ((((PRNG16 & 0xFF) >> 4) + (PRNG16 & 0x0F)) & 0x0F) + 0x08;
    uint32_t myclock30 = (uint32_t)((clock32 * myspeedmultiplierQ5_3) >> 3) + myclockoffset16;
    uint8_t  myunique8 = PRNG16 >> 8; // get 'salt' value for this pixel

    // We now have the adjusted 'clock' for this pixel, now we call
    // the function that computes what color the pixel should be based
    // on the "brightness = f( time )" idea.
    CRGB c = computeOneTwinkle( myclock30, myunique8);

    // If the new pixel is brighter than the background color, use it.
    if ( c.getAverageLight() > backgroundBrightness) {
      L[i] = c;
    } else {
      L[i] = gBackgroundColor;
    }
  }
}


//  This function takes a time in pseudo-milliseconds,
//  figures out brightness = f( time ), and also hue = f( time )
//  The 'low digits' of the millisecond time are used as
//  input to the brightness wave function.
//  The 'high digits' are used to select a color, so that the color
//  does not change over the course of the fade-in, fade-out
//  of one cycle of the brightness wave function.
//  The 'high digits' are also used to determine whether this pixel
//  should light at all during this cycle, based on the TWINKLE_DENSITY.
CRGB computeOneTwinkle( uint32_t ms, uint8_t salt)
{
  uint16_t ticks = ms >> (8 - TWINKLE_SPEED);
  uint8_t fastcycle8 = ticks;
  uint8_t slowcycle8 = (ticks >> 8) ^ salt;

  uint8_t bright = 0;
  if ( ((slowcycle8 & 0x0E) / 2) < TWINKLE_DENSITY) {
    bright = triwave8( fastcycle8);
  }

  uint8_t hue = (slowcycle8 * 16) + salt;
  return ColorFromPalette( gCurrentPalette, hue, bright, NOBLEND);
}


// A mostly red palette with green accents and white trim.
// "CRGB::Gray" is used as white to keep the brightness more uniform.
const TProgmemRGBPalette16 RedGreenWhite_p FL_PROGMEM =
{ CRGB::Red, CRGB::Red, CRGB::Red, CRGB::Red,
  CRGB::Red, CRGB::Red, CRGB::Red, CRGB::Red,
  CRGB::Red, CRGB::Red, CRGB::Gray, CRGB::Gray,
  CRGB::Green, CRGB::Green, CRGB::Green, CRGB::Green
};

// A red and white striped palette
// "CRGB::Gray" is used as white to keep the brightness more uniform.
const TProgmemRGBPalette16 RedWhite_p FL_PROGMEM =
{ CRGB::Red,  CRGB::Red,  CRGB::Red,  CRGB::Red,
  CRGB::Gray, CRGB::Gray, CRGB::Gray, CRGB::Gray,
  CRGB::Red,  CRGB::Red,  CRGB::Red,  CRGB::Red,
  CRGB::Gray, CRGB::Gray, CRGB::Gray, CRGB::Gray
};

// A mostly blue palette with white accents.
// "CRGB::Gray" is used as white to keep the brightness more uniform.
const TProgmemRGBPalette16 BlueWhite_p FL_PROGMEM =
{ CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::Blue,
  CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::Blue,
  CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::Blue,
  CRGB::Blue, CRGB::Gray, CRGB::Gray, CRGB::Gray
};

// A pure "fairy light" palette with some brightness variations
#define HALFFAIRY ((CRGB::FairyLight & 0xFEFEFE) / 2)
#define QUARTERFAIRY ((CRGB::FairyLight & 0xFCFCFC) / 4)
const TProgmemRGBPalette16 FairyLight_p FL_PROGMEM =
{ CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight,
  HALFFAIRY,        HALFFAIRY,        CRGB::FairyLight, CRGB::FairyLight,
  QUARTERFAIRY,     QUARTERFAIRY,     CRGB::FairyLight, CRGB::FairyLight,
  CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight
};

// A palette of soft snowflakes with the occasional bright one
const TProgmemRGBPalette16 Snow_p FL_PROGMEM =
{ 0x404040, 0x404040, 0x404040, 0x404040,
  0x404040, 0x404040, 0x404040, 0x404040,
  0x404040, 0x404040, 0x404040, 0x404040,
  0x404040, 0x404040, 0x404040, 0xFFFFFF
};

const TProgmemRGBPalette16 RGB_p FL_PROGMEM = {
  CRGB::Red,   CRGB::Green, CRGB::Blue,  CRGB::Red,
  CRGB::Green, CRGB::Blue,  CRGB::Red,   CRGB::Green,
  CRGB::Blue,  CRGB::Red,   CRGB::Green, CRGB::Blue,
  CRGB::Red,   CRGB::Green, CRGB::Blue,  CRGB::Red,
};

const TProgmemRGBPalette16 White_p FL_PROGMEM = {
  CRGB::Gray, CRGB::Gray, CRGB::Gray, CRGB::Gray,
  CRGB::Gray, CRGB::Gray, CRGB::Gray, CRGB::Gray,
  CRGB::Gray, CRGB::Gray, CRGB::Gray, CRGB::Gray,
  CRGB::Gray, CRGB::Gray, CRGB::Gray, CRGB::Gray
};

// Add or remove palette names from this list to control which color
// palettes are used, and in what order.
const TProgmemRGBPalette16* ActivePaletteList[] = {
  // &RainbowStripeColors_p,
  // &RedGreenWhite_p,
  // &RainbowStripeColors_p,
  &BlueWhite_p,
  // &RainbowStripeColors_p,
  &FairyLight_p,
  // &RainbowStripeColors_p,
  // &RedWhite_p,
  // &RainbowStripeColors_p,
  // &PartyColors_p,
  // &RainbowStripeColors_p,
  // &Snow_p,
};


// Advance to the next color palette in the list (above).
void chooseNextColorPalette( CRGBPalette16& pal)
{
  const uint8_t numberOfPalettes = sizeof(ActivePaletteList) / sizeof(ActivePaletteList[0]);
  static uint8_t whichPalette = -1;
  whichPalette = addmod8( whichPalette, 1, numberOfPalettes);

  pal = *(ActivePaletteList[whichPalette]);
}
