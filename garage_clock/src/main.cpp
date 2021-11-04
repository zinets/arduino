#include <Arduino.h>

#include "digits/digits.h"

RTC_DS1307 rtc;
// DateTime time;

#define DELAY 1500

volatile int b = 0;

IRAM_ATTR void updateTime() {
  b = 1;
}

void setup() {
  delay(1000);

  setupLed();

  Serial.begin(19200);
  Serial.println("setup begin");

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // rtc.writeSqwPinMode(DS1307_SquareWave1HZ);
  // pinMode(RTC_SQUARE_PIN, INPUT);
  // attachInterrupt(digitalPinToInterrupt(RTC_SQUARE_PIN), updateTime, RISING);

  Serial.println("setup done");
}



void loop() {
  // for (int i = 0; i < 10; i++) {
  //   shownum(i);
  
  //   delay(DELAY);
  // }

  DateTime time = rtc.now();
  Serial.println(String("DateTime::TIMESTAMP_TIME:\t")+time.timestamp(DateTime::TIMESTAMP_FULL));

  displayTime(time);

  delay(1000);

  // if (b) {
  //   Serial.println("piu");
  //   b = 0;
  // }
}