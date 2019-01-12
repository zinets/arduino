#include "hal.h"
#include "types.h"

#include <Bounce2.h>
#include <EEPROM.h>
#include <Wire.h>

#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <NtpClientLib.h>

// LCD SCREEN
LiquidCrystal lcd(LCD_PINS);
volatile LedState ledState = ledStateReady;
char buf[16];

// RGB LED
CRGB leds[NUM_LEDS];

// start button
Bounce debouncer = Bounce();

// realtime backup clock
RTC_DS1307 rtc;

// protos
void ping();
void updateColor();
void configModeCallback (WiFiManager *myWiFiManager);

// vars
struct TimeObject timeStruct;
volatile MainState mainState = stateInactive;
volatile DisplayState displayState = stateRemainTime;
int displayStateCounter = 0;
boolean syncEventTriggered = false;
NTPSyncEvent_t ntpEvent;

#define BLYNK_USE
#ifdef BLYNK_USE
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <BlynkSimpleEsp8266.h>
char  blynkToken[33] = "48ed6d953c2e4a5f8ff06fe5f2a8b415";
BlynkTimer timer;
#endif

bool isClockRunning = false;
time_t nowTime = 0;

void setup() {
  delay(1000);
  Serial.begin(9600);

  // lcd initialization
  lcd.begin(16, 2);
  lcd.createChar(0, divider);
  lcd.createChar(1, arrived);
  lcd.createChar(2, exit_symbol);
  lcd.createChar(3, clock_symbol);
  lcd.createChar(4, dinner_symbol);
  lcd.createChar(5, temp_symbol);

  #define DIVIDER_SYMBOL  byte(0)
  #define ARRIVED_SYMBOL  byte(1)
  #define EXIT_SYMBOL     byte(2)
  #define CLOCK_SYMBOL    byte(3)
  #define DINNER_SYMBOL   byte(4)
  #define TEMP_C_SYMBOL   byte(5)

Serial.println("lcd inited");

  // RGB-led initialization
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip);
 leds[0] = CRGB::Black;
 FastLED.show();
Serial.println("led inited");

  WiFiManager wifiManager;
  lcd.setCursor(0, 0);
  lcd.print("WiFi connect...");
  wifiManager.setBreakAfterConfig(true);
  wifiManager.setAPCallback(configModeCallback);

  // WiFiManagerParameter custom_blynk_text("<br/>Blynk config. <br/>");
  // wifiManager.addParameter(&custom_blynk_text);
  // WiFiManagerParameter custom_blynk_token("blynk-token", "blynk token", blynkToken, 33);
  // wifiManager.addParameter(&custom_blynk_token);

  if (!wifiManager.autoConnect()) {
    delay(3000);
    ESP.reset();
    delay(5000);
  }
Serial.println("wifi connected");

#ifdef BLYNK_USE
  Blynk.config(blynkToken);
#endif

  int const timeZone = 2;
  NTP.begin (DEFAULT_NTP_SERVER, timeZone, true);
  NTP.setInterval (1800);
  // NTP.onNTPSyncEvent ([](NTPSyncEvent_t event) {
  //     ntpEvent = event;
  //     syncEventTriggered = true;
  // });

Serial.println("timeserver ready");
nowTime = NTP.getTime();
Serial.print("#1 ");
Serial.println(NTP.getTimeDateString(nowTime));
DateTime now = DateTime(nowTime);
  // start button
  pinMode(BUTTON_PIN, INPUT);
  debouncer.attach(BUTTON_PIN);
  debouncer.interval(10);
Serial.println("button ready");

  // EEPROM
  EEPROM.begin(sizeof(TimeObject));
  if (digitalRead(BUTTON_PIN)) {
    timeStruct.timerStarted = false;
    lcd.setCursor(0, 0);
    lcd.print("Cleared!");
    while (digitalRead(BUTTON_PIN));
  } else {
    EEPROM.get(EEPROM_ADDR, timeStruct);
    lcd.print("Read.");
  }

  // rtc
  Wire.begin();
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // rtc.adjust(DateTime(__DATE__, __TIME__));
    timeStruct.timerStarted = false;

    nowTime = NTP.getTime();
    now = DateTime(nowTime);
    Serial.print("#2 ");
    Serial.println(NTP.getTimeDateString(nowTime));
  }
  rtc.adjust(now);

  if (rtc.isrunning()) {
    Serial.println("RTC inited & updated");
    isClockRunning = true;
    // Serial.println("#3 " + String() + ":" + String());
  } else {
    Serial.println("WTF??");
  }

  // 1 Hz timer
  // rtc.writeSqwPinMode(SquareWave1HZ);
//   pinMode(RTC_SQUARE_PIN, INPUT);
//   attachInterrupt(digitalPinToInterrupt(RTC_SQUARE_PIN), ping, RISING);
// Serial.println("1 Hz timer enabled");

  lcd.clear();
  int timerID = timer.setInterval(1000L, ping);
}

bool state = false;

void loop() {
  switch (mainState) {
    case stateTimeUpdated: {
      DateTime now;
      if (isClockRunning) {
        now = rtc.now();
      } else {
        now = DateTime(nowTime);
      }
      // print date
      int sec = now.second();
      sprintf(buf, "%02d.%02d   ", now.day(), now.month());//, now.year() - 2000); to clear ending "Clear-ed!"
      lcd.setCursor(0, 0);
      lcd.print(buf);

      // print current time
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
          switch (displayState) {
            case stateRemainTime:
              if (dotsOn) {
                sprintf(buf, "%c %02d %02d", EXIT_SYMBOL, remain.hours(), remain.minutes());
              } else {
                sprintf(buf, "%c %02d:%02d", EXIT_SYMBOL, remain.hours(), remain.minutes());
              }
              break;
            case stateFinishTime:
              sprintf(buf, "%c %02d:%02d", CLOCK_SYMBOL, timeStruct.endTime.hour(), timeStruct.endTime.minute());
              break;
          }
          lcd.print(buf);
        } else {
          timeStruct.timerStarted = false;

          lcd.setCursor(9, 0);
          lcd.print("       ");
          lcd.setCursor(9, 1);
          lcd.print("       ");
          EEPROM.put(EEPROM_ADDR, timeStruct);
          EEPROM.commit();
        }
      }
      if (dinnerTime) {
        ledState = ledStateDinner;
      } else if (timeStruct.timerStarted) {
        ledState = ledStateWork;
      } else {
        ledState = ledStateReady;
      }

      mainState = stateInactive;
    } break;
    case stateInactive:
    default:
      // таймер не запущен и нажата кнопка - запуск таймера
      if (!timeStruct.timerStarted && debouncer.update()) {
        if (debouncer.rose()) {
          timeStruct.timerStarted = true;
          timeStruct.arriveTime = rtc.now();
          timeStruct.endTime = timeStruct.arriveTime + TimeSpan(9 * 60 * 60 + 59);
          EEPROM.put(EEPROM_ADDR, timeStruct);
          EEPROM.commit();
        }
      }

      break;
  }

  updateColor();
  #ifdef BLYNK_USE
  Blynk.run();
  timer.run();
  #endif
}

// обработчик 1 Гц прерывания от rtc
void ping() {
  Serial.println("!"); 
  mainState = stateTimeUpdated;
  nowTime++;

  if (displayStateCounter++ % 5 == 0) {
    switch (displayState) {
      case stateRemainTime:
        displayState = stateFinishTime;
        break;
      case stateFinishTime:
        displayState = stateRemainTime;
        break;
    }
  }

  Serial.println("!");

#ifdef BLYNK_USE
  // String arrived = String(timeStruct.arriveTime.hour()) + ":" + String(timeStruct.arriveTime.minute());
  // Blynk.virtualWrite(V0, arrived);
  // Blynk.virtualWrite(V1, String(timeStruct.endTime.hour()) + ":" + String(timeStruct.endTime.minute()));
#endif
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

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("failed to connect, we should reset as see if it connects");
  lcd.setCursor(0, 0);
  lcd.print("Needs wifi setup");
  lcd.setCursor(0, 1);
  lcd.print(myWiFiManager->getConfigPortalSSID());
}
