#include <Arduino.h>

#include "digits/digits.h"

RTC_DS1307 rtc;

#define DELAY 1500

void setup() {
  setupLed();

  Serial.begin(115200);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
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
}