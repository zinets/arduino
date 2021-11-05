#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <WiFiUdp.h>
#include <NTPClient.h>

char ssid[] = "OnePlus 7";
char pass[] = "134679852";    

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

  Serial.begin(19200);
  Serial.println("setup begin");

  WiFi.begin(ssid, pass);
  int i = 10;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (--i < 0) {
      break;
    }
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Wifi connected");

    const long utcOffsetInSeconds = 3 * 3600;
    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
    
    timeClient.begin();
    timeClient.update();

    Serial.println(timeClient.getFormattedTime());

    timeClient.end();
  }

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

  setupLed();

  Serial.println("setup done");
}



void loop() {
  DateTime time = rtc.now();
  Serial.println(String("DateTime::TIMESTAMP_TIME:\t")+time.timestamp(DateTime::TIMESTAMP_FULL));

  displayTime(time);

  delay(1000);
}