#ifndef _led_H
#define _led_H

#define FASTLED_ESP8266_D1_PIN_ORDER
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

#define NUM_LEDS            103
#define FRAMES_PER_SECOND   60
// частота вспышек (** против 255)
#define GLITTER_INTENSITY   20
#define COMMON_FREQUENCY    99

extern CRGB leds[NUM_LEDS];
extern uint8_t gHue; 

// gold stipes
#define NUM_GOLD_LEDS       149
extern CRGB goldLeds[NUM_GOLD_LEDS];
extern uint8_t goldHue; 

#define BRIGHTNES_DAY       200
#define BRIGHTNES_NIGHT     100

void setupLeds();
void handleLights();

#endif