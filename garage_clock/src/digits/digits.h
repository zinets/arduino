#ifndef _digits
#define _digits

#include <FastLED.h>
#include "RTClib.h"

#define NUM_LEDS 7
#define DATA_PIN D4
#define FASTLED_ESP8266_D1_PIN_ORDER

// rtc
#define RTC_SDA         D2
#define RTC_SCL         D1
#define RTC_SQUARE_PIN  D3

void setupLed();
void shownum(int number);
// показываем время - точки тупо показывать для четных секунд
void displayTime(DateTime time);

#endif