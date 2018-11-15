#include "hal.h"
#include "types.h"

#include <Bounce2.h>

#include <Wire.h>
#include "RTClib.h"

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
void ping();

void setup() {
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

  // start button
  pinMode(BUTTON_PIN, INPUT);
  debouncer.attach(BUTTON_PIN);
  debouncer.interval(10);
Serial.println("button ready");

  // rtc
  Wire.begin();
  rtc.begin();
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }
Serial.println("RTC inited")  ;

  // 1 Hz timer
  rtc.writeSqwPinMode(SquareWave1HZ);
  pinMode(RTC_SQUARE_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(RTC_SQUARE_PIN), ping, RISING);
Serial.println("1 Hz timer enabled");
}

bool state = false;

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("started");


  debouncer.update();
  if (debouncer.rose()) {
    state = !state;
  }

  // if (!state) {
  //   leds[0] = CRGB::Green; //color;
  // } else {
  //   leds[0] = CRGB::Magenta;
  // }
  // FastLED.show();

  DateTime now = rtc.now();
  // print date
  int sec = now.second();
  // print current time
  bool dotsOn = sec % 2 == 0;
  byte dividerSym = dotsOn ? ':' : ' ';
  sprintf(buf, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());

  lcd.setCursor(0, 1);
  lcd.print(buf);
}

// обработчик 1 Гц прерывания от rtc
void ping() {
  Serial.print("!");
  state = !state;
}
