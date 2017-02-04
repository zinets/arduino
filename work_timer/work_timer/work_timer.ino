#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include "RTClib.h"

LiquidCrystal lcd(12, 11, 9, 8, 7, 6);
RTC_DS1307 rtc;

enum MainState {
    stateTimeUpdated,
    stateInactive,
};
volatile MainState mainState = stateInactive;

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
  attachInterrupt(0, ping, RISING);
}

void loop() {
  switch (mainState) {
    case stateTimeUpdated: {
      DateTime now = rtc.now();
      String time;
      int sec = now.second();
      if (sec % 2 == 0) {
        time = String(now.hour()) + ":" + String(now.minute());
      } else {
        time = String(now.hour()) + " " + String(now.minute());
      }
      lcd.setCursor(0, 1);
      lcd.print(time);

      mainState = stateInactive;
    } break;
    case stateInactive:
    default:

      break;
  }
}

void ping() {
  mainState = stateTimeUpdated;
}
