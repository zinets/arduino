#ifndef TYPES_H
#define TYPES_H

#include <LiquidCrystal.h>
#include "FastLED.h"
#include "RTClib.h"

// LCD
enum LedState {
  ledStateReady,
  ledStateWork,
  ledStateDinner,
};

enum MainState {
    stateTimeUpdated,
    stateInactive,
};

enum DisplayState {
  stateRemainTime, // оставшееся время в часах
  stateFinishTime, // время ухода в ЧЧ:ММ
};

typedef struct TimeObject {
  bool timerStarted;
  DateTime arriveTime;
  DateTime endTime;
} TimeObject;

// LED
const TProgmemRGBPalette16 WorkColors_p FL_PROGMEM = {
    CRGB::Red,
    CRGB::Orange,
    CRGB::Maroon,
    CRGB::Red,

    CRGB::Red,
    CRGB::Orange,
    CRGB::Maroon,
    CRGB::DarkRed,

    CRGB::Red,
    CRGB::Orange,
    CRGB::Maroon,
    CRGB::Red,

    CRGB::Red,
    CRGB::Orange,
    CRGB::Maroon,
    CRGB::DarkRed,
};

const TProgmemRGBPalette16 ReadyColors_p FL_PROGMEM = {
    CRGB::DarkGreen,
    CRGB::DarkGreen,
    CRGB::DarkOliveGreen,
    CRGB::DarkGreen,

    CRGB::Green,
    CRGB::ForestGreen,
    CRGB::OliveDrab,
    CRGB::Green,

    CRGB::SeaGreen,
    CRGB::MediumAquamarine,
    CRGB::LimeGreen,
    CRGB::YellowGreen,

    CRGB::LightGreen,
    CRGB::LawnGreen,
    CRGB::MediumAquamarine,
    CRGB::ForestGreen
};


byte divider[8] = {
  B01100,
  B01100,
  B01100,
  B01100,
  B01100,
  B01100,
  B01100,
  B01100,
};

byte arrived[8] = {
  B00000,
  B00100,
  B00010,
  B11111,
  B00010,
  B00100,
  B00000,
  B00000,
};

byte exit_symbol[8] = {
  B00000,
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00000,
};

byte clock_symbol[8] = {
  B01110,
  B10101,
  B10101,
  B10111,
  B10001,
  B10001,
  B01110,
  B00000,
};

byte dinner_symbol[8] = {
  B10100,
  B01010,
  B00101,
  B00000,
  B00111,
  B01111,
  B10111,
  B01111,
};

byte temp_symbol[8] = {
  B00010,
  B00101,
  B00010,
  B01000,
  B11100,
  B01000,
  B01000,
  B00110,
};

#endif
