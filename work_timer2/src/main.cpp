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
Serial.println("led inited");

  // start button
  pinMode(BUTTON_PIN, INPUT);
  debouncer.attach(BUTTON_PIN);
  debouncer.interval(10); 
Serial.println("button ready");

  // rtc
//   Wire.begin();
//   rtc.begin();
//   if (!rtc.isrunning()) {
//     Serial.println("RTC is NOT running!");
//     rtc.adjust(DateTime(__DATE__, __TIME__));
//   }
// Serial.println("RTC inited")  ;

//   // 1 Hz timer
//   rtc.writeSqwPinMode(SquareWave1HZ);
//   attachInterrupt(0, ping, RISING);
// Serial.println("1 Hz timer enabled");
}

bool state = false;

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("started");


  debouncer.update();    
  if (debouncer.rose()) {
    state = !state;
  }

  if (!state) {
    leds[0] = CRGB::Green; //color;
  } else {
    leds[0] = CRGB::Magenta;
  }
  FastLED.show();
  
}
