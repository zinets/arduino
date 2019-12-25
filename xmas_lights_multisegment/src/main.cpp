// OTA support
#ifdef TARGET_ESP8266
#define OTA

#ifdef OTA
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#endif


#endif

#define FASTLED_ESP8266_D1_PIN_ORDER
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

#define NUM_LEDS            103
#define FRAMES_PER_SECOND   60
// скорость изменения цвета
#define COLOR_CHANGE_SPEED  50 
// частота вспышек
#define GLITTER_FREQUENCY   100
// яркость вспышек
#define GLITTER_INTENSITY   65

#define COMMON_FREQUENCY    99

CRGB leds[NUM_LEDS];
uint8_t gHue = 0; 


// gold stipes
#define NUM_GOLD_LEDS       149
CRGB goldLeds[NUM_GOLD_LEDS];
uint8_t goldHue = 0; 

void setup() {

  #ifdef TARGET_ESP8266

  #ifdef OTA
  // TODO: WiFi-manager
  const char* ssid = "Hamster-wifi";
  const char* password = "134679852";
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    // Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  ArduinoOTA.setHostname("xtree");

  ArduinoOTA.begin();
  // Serial.println("Ready");
  // Serial.print("IP address: ");
  // Serial.println(WiFi.localIP());
  #endif
  #endif

  FastLED.addLeds<NEOPIXEL, 7>(leds, 0, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 5>(goldLeds, 0, NUM_GOLD_LEDS);

  FastLED.setBrightness(220); // 0 - 255
}

void addGlitter(fract8 chanceOfGlitter) {
  if (random8() < chanceOfGlitter) {
    leds[random16(NUM_LEDS)] += CRGB::White;
  }
}

// void rainbowWithGlitter() {
//   fill_rainbow( leds, NUM_LEDS, gHue, 2);
//   EVERY_N_MILLISECONDS( GLITTER_FREQUENCY ) { 
//     addGlitter(GLITTER_INTENSITY); 
//   }   
// }

CRGB gBackgroundColor = CRGB::Black;

const TProgmemRGBPalette16 Gold_p FL_PROGMEM = { 
  CRGB::Orange, CRGB::Orange, CRGB::Orange, CRGB::Orange,
  CRGB::Orange, CRGB::Orange, CRGB::Orange, CRGB::Orange,
  CRGB::Orange, CRGB::Orange, CRGB::Orange, CRGB::Orange,
  CRGB::Orange, CRGB::Orange, CRGB::Orange, CRGB::Orange,  
};
CRGBPalette16 gCurrentPalette = Gold_p;

#define TWINKLE_SPEED         1
#define TWINKLE_DENSITY       15

CRGB computeOneTwinkle( uint32_t ms, uint8_t salt) {
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

void drawTwinkles(CRGB* L, uint16_t N) {
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

void loop() {
  fill_rainbow( leds, NUM_LEDS, gHue, 2);
  drawTwinkles(goldLeds, NUM_GOLD_LEDS);

  EVERY_N_MILLISECONDS (COMMON_FREQUENCY) {
    gHue++;
    goldHue++;
    addGlitter(GLITTER_INTENSITY); 
  }

  FastLED.show();  
  FastLED.delay(1000 / FRAMES_PER_SECOND); 

  #ifdef TARGET_ESP8266
  #ifdef OTA
    ArduinoOTA.handle();
  #endif
  #endif
}