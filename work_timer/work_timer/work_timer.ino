#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Bounce2.h>
#include <EEPROM.h>

#include "RTClib.h"

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

enum MainState {
    stateTimeUpdated,
    stateInactive,
};
volatile MainState mainState = stateInactive;

typedef struct TimeObject {
  bool timerStarted;
  DateTime arriveTime;
  DateTime endTime;
} TimeObject;

struct TimeObject timeStruct;

void setup() {
  Serial.begin(57600);
  Wire.begin();
  rtc.begin();
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }
  rtc.writeSqwPinMode(SquareWave1HZ);

  lcd.begin(16, 2);
  lcd.createChar(0, divider);
  lcd.createChar(1, arrived);
  lcd.createChar(2, exit_symbol);

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
      lcd.setCursor(0, 0);
      lcd.print(String(now.day()) + "." + String(now.month()) + "." + String(now.year() - 2000) + "    ");
      // print current time
      int sec = now.second();
      if (sec % 2 == 0) {
        sprintf(buf, "%02d:%02d", now.hour(), now.minute());
      } else {
        sprintf(buf, "%02d %02d", now.hour(), now.minute());
      }
      lcd.setCursor(0, 1);
      lcd.print(buf);
      if (timeStruct.timerStarted) {
        TimeSpan remain = timeStruct.endTime - now;
        if (remain.totalseconds() > 1) {
          // print arrive time
          lcd.setCursor(9, 0);
          sprintf(buf, "%c %02d:%02d", byte(1), timeStruct.arriveTime.hour(), timeStruct.arriveTime.minute());
          lcd.print(buf);
          // print remain time
          lcd.setCursor(9, 1);
          sprintf(buf, "%c %02d:%02d", byte(2), remain.hours(), remain.minutes());
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
}

void ping() {
  mainState = stateTimeUpdated;
}
