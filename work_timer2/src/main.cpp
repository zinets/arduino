#include "hal.h"
#include "types.h"

// LCD SCREEN
LiquidCrystal lcd(LCD_PINS);
volatile LedState ledState = ledStateReady;
char buf[16];

// RGB LED
CRGB leds[NUM_LEDS];

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

}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("started");

  leds[0] = CRGB::Orange; //color;
  FastLED.show();
  delay(500);
  leds[0] = CRGB::Black;
  FastLED.show();
  delay(500);
}
