#include <Arduino.h>

#include <Wire.h>
#include <Bounce2.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "FastLED.h"
#include "RTClib.h"

#include "consts.h"
#include "hardware.h"

CRGB leds[NUM_LEDS];


volatile LedState ledState = ledStateReady;


#ifdef TARGET_AVR
#include <LiquidCrystal.h>
rs, en, d0, d1, d2, d3
LiquidCrystal lcd(12, 11, 9, 8, 7, 6);
#endif

#ifdef TARGET_ESP8266
#include "LiquidCrystal.h"
//Define 74HC595 Connections
const int Clock = D5;
const int Data = D6;
const int Latch = D7;

LiquidCrystal lcd(D5, D6, D7);
#endif

// RTC_DS1307 rtc;
char buf[16];
Bounce bouncer = Bounce();

volatile MainState mainState = stateInactive;

volatile DisplayState displayState = stateRemainTime;
int displayStateCounter = 0;
 
struct TimeObject timeStruct;

// OneWire oneWire(ONE_WIRE_BUS);
// DallasTemperature sensors(&oneWire);
// DeviceAddress insideThermometer;
// bool oneWireInited = false;
// double currentTemp = 0;

void setup() {
  Serial.begin(9600);

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

  Serial.print("Locating devices...");

  // sensors.begin();
  // Serial.print("Found ");
  // Serial.print(sensors.getDeviceCount(), DEC);
  // Serial.println(" devices.");

  // Serial.print("Parasite power is: ");
  // if (sensors.isParasitePowerMode())
  //   Serial.println("ON");
  // else
  //   Serial.println("OFF");

  // if (!sensors.getAddress(insideThermometer, 0)) {
  //   Serial.println("Unable to find address for Device 0");
  // } else {
  //   oneWireInited = true;
  //   Serial.print("Device 0 Address: ");
  //   printAddress(insideThermometer);
  //   Serial.println();
  // }
  // currentTemp = updateTemperature();

  // Wire.begin();
  // rtc.begin();
  // if (!rtc.isrunning()) {
  //   Serial.println("RTC is NOT running!");
  //   rtc.adjust(DateTime(__DATE__, __TIME__));
  // }
  // rtc.writeSqwPinMode(SquareWave1HZ);

  // // led
  // FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  

  attachInterrupt(0, ping, RISING);

  pinMode(BUTTON_PIN, INPUT);

  if (digitalRead(BUTTON_PIN)) {
    timeStruct.timerStarted = false;
    lcd.setCursor(0, 0);
    lcd.print("Cleared!");
    while (digitalRead(BUTTON_PIN));
  } else {
    lcd.setCursor(0, 1);
    lcd.print("loaded");
    EEPROM.get(EEPROM_ADDR, timeStruct);
  }
  bouncer.attach(BUTTON_PIN);
  bouncer.interval(5);
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("test");

  // switch (mainState) {
  //   case stateTimeUpdated: {
  //     DateTime now = rtc.now();
  //     // print date
  //     int sec = now.second();
  //     sprintf(buf, "%02d.%02d   ", now.day(), now.month());//, now.year() - 2000); to clear ending "Clear-ed!"
  //     lcd.setCursor(0, 0);
  //     lcd.print(buf);

  //     // print current time
  //     bool dotsOn = sec % 2 == 0;
  //     bool dinnerTime = now.hour() == 12 && now.minute() > 35 && now.minute() < 59;
  //     byte dinnerSym = dinnerTime ? DINNER_SYMBOL : ' ';
  //     byte dividerSym = dotsOn ? ':' : ' ';
  //     sprintf(buf, "%02d%c%02d %c", now.hour(), dividerSym, now.minute(), dinnerSym);

  //     lcd.setCursor(0, 1);
  //     lcd.print(buf);
  //     if (timeStruct.timerStarted) {
  //       TimeSpan remain = timeStruct.endTime - now;
  //       if (remain.totalseconds() > 1) {
  //         // print arrive time
  //         lcd.setCursor(9, 0);
  //         sprintf(buf, "%c %02d:%02d", ARRIVED_SYMBOL, timeStruct.arriveTime.hour(), timeStruct.arriveTime.minute());
  //         lcd.print(buf);
  //         // print remain time
  //         lcd.setCursor(9, 1);
  //         switch (displayState) {
  //           case stateRemainTime:
  //             if (dotsOn) {
  //               sprintf(buf, "%c %02d %02d", EXIT_SYMBOL, remain.hours(), remain.minutes());
  //             } else {
  //               sprintf(buf, "%c %02d:%02d", EXIT_SYMBOL, remain.hours(), remain.minutes());
  //             }
  //             break;
  //           case stateFinishTime:
  //             sprintf(buf, "%c %02d:%02d", CLOCK_SYMBOL, timeStruct.endTime.hour(), timeStruct.endTime.minute());
  //             break;
  //           case stateShowTemp:
  //             sprintf(buf, "%c %02d C ", TEMP_C_SYMBOL, (int)currentTemp);
  //             break;
  //         }
  //         lcd.print(buf);
  //       } else {
  //         timeStruct.timerStarted = false;

  //         lcd.setCursor(9, 0);
  //         lcd.print("       ");
  //         lcd.setCursor(9, 1);
  //         lcd.print("       ");
  //         EEPROM.put(EEPROM_ADDR, timeStruct);
  //       }
  //     }
  //     if (dinnerTime) {
  //       ledState = ledStateDinner;
  //     } else if (timeStruct.timerStarted) {
  //       ledState = ledStateWork;
  //     } else {
  //       ledState = ledStateReady;
  //     }

  //     if (now.second() == 0 && (now.minute() % 5 == 0)) {
  //       currentTemp = updateTemperature();
  //     }

  //     mainState = stateInactive;
  //   } break;
  //   case stateInactive:
  //   default:
  //     // таймер не запущен и нажата кнопка - запуск таймера
  //     if (!timeStruct.timerStarted && bouncer.update()) {
  //       if (bouncer.fell()) {
  //         timeStruct.timerStarted = true;
  //         timeStruct.arriveTime = rtc.now();
  //         timeStruct.endTime = timeStruct.arriveTime + TimeSpan(9 * 60 * 60 + 59);
  //         EEPROM.put(EEPROM_ADDR, timeStruct);
  //       }
  //     }
  //     // таймер запущен и нажата кнопка - посылка в порт сигнала
  //     else if (timeStruct.timerStarted && bouncer.update()) {
  //       if (bouncer.fell()) {
  //         Serial.println("screenOff");
  //       }
  //     }
  //     break;
  // }

  // if (Serial.available() > 0) {
  //   if (Serial.read() == 't') { // marker for correction time
  //     // Serial.println("reading..");
  //     String nowUnixTime = Serial.readString();
  //     Serial.println("readed: " + nowUnixTime);
  //     DateTime nowDt = DateTime(nowUnixTime.toInt());
  //     rtc.adjust(nowDt);
  //     Serial.println(String(nowDt.hour()) + ":" + String(nowDt.minute()));
  //   }
  // }
  // updateColor();
}

void ping() {
  mainState = stateTimeUpdated;

  if (displayStateCounter++ % 5 == 0) {
    switch (displayState) {
      case stateRemainTime:
        displayState = stateFinishTime;
        break;
      case stateFinishTime:
        displayState = stateShowTemp;
        break;
      case stateShowTemp:
        displayState = stateRemainTime;
        break;
    }
  }
}

// void updateColor() {
//   static uint8_t index = 0;

//   switch (ledState) {
//     case ledStateWork: {
//       CRGB color = ColorFromPalette(WorkColors_p, index);
//       leds[0] = color;
//     } break;
//     case ledStateReady:
//     default: {
//       CRGB color = ColorFromPalette(ReadyColors_p, index);
//       leds[0] = color;
//     } break;
//   }

//   EVERY_N_MILLISECONDS(40) {
//     index++;
//     FastLED.show();
//   }
// }

// void printAddress(DeviceAddress deviceAddress) {
//   for (uint8_t i = 0; i < 8; i++) {
//     if (deviceAddress[i] < 16) Serial.print("0");
//     Serial.print(deviceAddress[i], HEX);
//   }
// }

// float updateTemperature() {
//   float tempC = 0;
//   if (oneWireInited) {
//     // Serial.print("Requesting temperatures...");
//     sensors.requestTemperatures(); // Send the command to get temperatures
//     // Serial.println("DONE");

//     tempC = sensors.getTempC(insideThermometer);
//     // Serial.print("Temp C: ");
//     // Serial.print(tempC);
//   }
//   return tempC;
// }
