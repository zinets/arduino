#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Bounce2.h>
#include <EEPROM.h>

#include "FastLED.h"
#include "RTClib.h"

#define NUM_LEDS 1
#define DATA_PIN 4
CRGB leds[NUM_LEDS];

enum LedState {
  ledStateReady,
  ledStateWork,
  ledStateDinner,
};
volatile LedState ledState = ledStateReady;

const TProgmemRGBPalette16 WorkColors_p FL_PROGMEM = {
    CRGB::Orange,
    CRGB::Maroon,

    CRGB::DarkRed,
    CRGB::Maroon,
    CRGB::DarkRed,

    CRGB::DarkRed,
    CRGB::DarkRed,
    CRGB::Red,
    CRGB::Orange,

    CRGB::White,
    CRGB::Orange,
    CRGB::Red,
    CRGB::DarkRed
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

LiquidCrystal lcd(12, 11, 9, 8, 7, 6);
RTC_DS1307 rtc;
char buf[16];
Bounce bouncer = Bounce();
#define BUTTON_PIN    3
#define EEPROM_ADDR   0

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

enum MainState {
    stateTimeUpdated,
    stateInactive,
};
volatile MainState mainState = stateInactive;

enum DisplayState {
  stateRemainTime, // оставшееся время в часах
  stateFinishTime, // время ухода в ЧЧ:ММ
};
volatile DisplayState displayState = stateRemainTime;
int displayStateCounter = 0;

typedef struct TimeObject {
  bool timerStarted;
  DateTime arriveTime;
  DateTime endTime;
} TimeObject;

struct TimeObject timeStruct;

void setup() {
  Serial.begin(115200);

  Wire.begin();
  rtc.begin();
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }
  rtc.writeSqwPinMode(SquareWave1HZ);

  // led
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  lcd.begin(16, 2);
  lcd.createChar(0, divider);
  lcd.createChar(1, arrived);
  lcd.createChar(2, exit_symbol);
  lcd.createChar(3, clock_symbol);
  lcd.createChar(4, dinner_symbol);

  #define DIVIDER_SYMBOL  byte(0)
  #define ARRIVED_SYMBOL  byte(1)
  #define EXIT_SYMBOL     byte(2)
  #define CLOCK_SYMBOL    byte(3)
  #define DINNER_SYMBOL   byte(4)

  attachInterrupt(0, ping, RISING);

  pinMode(BUTTON_PIN, INPUT);

  if (digitalRead(BUTTON_PIN)) {
    timeStruct.timerStarted = false;
    lcd.setCursor(0, 0);
    lcd.print("Cleared!");
    while (digitalRead(BUTTON_PIN));
  } else {
    EEPROM.get(EEPROM_ADDR, timeStruct);
  }
  bouncer.attach(BUTTON_PIN);
  bouncer.interval(5);
}

void loop() {
  switch (mainState) {
    case stateTimeUpdated: {
      DateTime now = rtc.now();
      // print date

      // lcd.print(String(now.day()) + "." + String(now.month()) + "." + String(now.year() - 2000) + "    ");

      sprintf(buf, "%02d.%02d   ", now.day(), now.month());//, now.year() - 2000); to clear ending "Clear-ed!"
      lcd.setCursor(0, 0);
      lcd.print(buf);

      // print current time
      int sec = now.second();
      bool dotsOn = sec % 2 == 0;
      bool dinnerTime = now.hour() == 12 && now.minute() > 35 && now.minute() < 59;
      byte dinnerSym = dinnerTime ? DINNER_SYMBOL : ' ';
      byte dividerSym = dotsOn ? ':' : ' ';      
      sprintf(buf, "%02d%c%02d %c", now.hour(), dividerSym, now.minute(), dinnerSym);

      lcd.setCursor(0, 1);
      lcd.print(buf);
      if (timeStruct.timerStarted) {
        TimeSpan remain = timeStruct.endTime - now;
        if (remain.totalseconds() > 1) {
          // print arrive time
          lcd.setCursor(9, 0);
          sprintf(buf, "%c %02d:%02d", ARRIVED_SYMBOL, timeStruct.arriveTime.hour(), timeStruct.arriveTime.minute());
          lcd.print(buf);
          // print remain time
          lcd.setCursor(9, 1);
          if (displayState == stateRemainTime) {
            if (dotsOn) {
              sprintf(buf, "%c %02d %02d", EXIT_SYMBOL, remain.hours(), remain.minutes());
            } else {
              sprintf(buf, "%c %02d:%02d", EXIT_SYMBOL, remain.hours(), remain.minutes());
            }
          } else {
            sprintf(buf, "%c %02d:%02d", CLOCK_SYMBOL, timeStruct.endTime.hour(), timeStruct.endTime.minute());
          }

          lcd.print(buf);
        } else {
          timeStruct.timerStarted = false;

          lcd.setCursor(9, 0);
          lcd.print("       ");
          lcd.setCursor(9, 1);
          lcd.print("       ");
          EEPROM.put(EEPROM_ADDR, timeStruct);
        }        
      }
      if (dinnerTime) {
        ledState = ledStateDinner;
      } else if (timeStruct.timerStarted) {
        ledState = ledStateWork;
      } else {
        ledStateReady;
      }
      
      mainState = stateInactive;
    } break;
    case stateInactive:
    default:
      if (!timeStruct.timerStarted && bouncer.update()) {
        if (bouncer.fell()) {
          timeStruct.timerStarted = true;
          timeStruct.arriveTime = rtc.now();
          timeStruct.endTime = timeStruct.arriveTime + TimeSpan(9 * 60 * 60 + 59);
          EEPROM.put(EEPROM_ADDR, timeStruct);
        }
      }
      break;
  }

  if (Serial.available() > 0) {
    if (Serial.read() == 't') { // marker for correction time
      // Serial.println("reading..");
      String nowUnixTime = Serial.readString();
      Serial.println("readed: " + nowUnixTime);
      DateTime nowDt = DateTime(nowUnixTime.toInt());
      rtc.adjust(nowDt);
      Serial.println(String(nowDt.hour()) + ":" + String(nowDt.minute()));      
    }
  }
  updateColor();
}

void ping() {
  mainState = stateTimeUpdated;
  // смена варианта отображения кажд. 3 тика
  if (displayStateCounter++ % 5 == 0) {
    if (displayState == stateRemainTime) {
      displayState = stateFinishTime;
    } else {
      displayState = stateRemainTime;
    }
  }
}

void updateColor() {
  static uint8_t index = 0;
  
  switch (ledState) {
    case ledStateWork: {
      CRGB color = ColorFromPalette(WorkColors_p, index);
      leds[0] = color;
    } break;
    case ledStateReady:
    default: {
      CRGB color = ColorFromPalette(ReadyColors_p, index);
      leds[0] = color;
    } break;
  }
  
  EVERY_N_MILLISECONDS(40) {    
    index++;
    FastLED.show();
  }  
}
