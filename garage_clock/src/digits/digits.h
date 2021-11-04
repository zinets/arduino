#ifndef _digits
#define _digits

#include <FastLED.h>

#define NUM_LEDS 7
#define DATA_PIN D4
#define FASTLED_ESP8266_D1_PIN_ORDER



void setupLed();
void shownum(int number);

#endif