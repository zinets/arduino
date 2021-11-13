#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <WiFiUdp.h>
#include <NTPClient.h>

char ssid[] = "OnePlus 7";
char pass[] = "134679852";    

#include "digits/digits.h"

RTC_DS1307 rtc;

#define DELAY 1500

volatile bool ready = true;

IRAM_ATTR void updateTime() {
  ready = true;
}

void setup() {
  delay(2000);

  Serial.begin(19200);
  Serial.println("setup begin");

  setupLed();
  showStartScreen();
  delay(500);

  DateTime defaultDate = DateTime(F(__DATE__), F(__TIME__));
  if (!rtc.begin()) {
    showErrorScreen();
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(defaultDate);
  }

  DateTime time = rtc.now();
  displayTime(time);

  WiFi.begin(ssid, pass);
  int i = 30;
  while (WiFi.status() != WL_CONNECTED) {
    delay(650);
    Serial.println(".");
    if (--i < 0) {
      break;
    }
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Wifi connected");

    const long utcOffsetInSeconds = 2 * 3600;
    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
    
    timeClient.begin();
    timeClient.update();

    defaultDate = DateTime(timeClient.getEpochTime());
    Serial.println(String("set date:\t") + defaultDate.timestamp(DateTime::TIMESTAMP_FULL));
    rtc.adjust(defaultDate);

    timeClient.end();
    WiFi.disconnect();
  }

  pinMode(RTC_SQUARE_PIN, INPUT);
  rtc.writeSqwPinMode(DS1307_SquareWave1HZ);
  attachInterrupt(digitalPinToInterrupt(RTC_SQUARE_PIN), updateTime, RISING);

  Serial.println("setup done");
}

void loop() {
  // DateTime time = rtc.now();
  // Serial.println(String("DateTime::TIMESTAMP_TIME:\t")+time.timestamp(DateTime::TIMESTAMP_FULL));

  // displayTime(time);

  // delay(1000);
  if (ready) {
    ready = false;

    DateTime time = rtc.now();
    displayTime(time);
  }
}