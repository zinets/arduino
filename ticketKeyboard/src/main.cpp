#include <Arduino.h>
#include <EEPROM.h>

#include <Adafruit_SH1106.h>
#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);

#include <Bounce2.h>
#define BOUNCE_PIN_PLUS   2
#define BOUNCE_PIN_MINUS  3
#define BOUNCE_PIN_RESET  4

Bounce b_plus = Bounce();
Bounce b_minus = Bounce();
Bounce b_reset = Bounce();

const int counter_addr = 0;
const int max_count_addr = 1;

void drawCurrent(int num, int from);

void setCounter(int newValue);
int getCounter();

void setMaxCount(int newValue);
int getMaxCount();

void setup() {
  b_plus.attach(BOUNCE_PIN_PLUS, INPUT_PULLUP);
  b_plus.interval(5);
  b_minus.attach(BOUNCE_PIN_MINUS, INPUT_PULLUP);
  b_minus.interval(5);

  b_reset.attach(BOUNCE_PIN_RESET, INPUT_PULLUP);
  b_reset.interval(5);

  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  
  display.setRotation(2);
  display.setTextColor(WHITE);
  
  // setCounter(0);
  // setMaxCount(130);
  drawCurrent(getCounter(), getMaxCount());

  Serial.begin(9600);
}

void drawCurrent(int num, int from) {

  display.clearDisplay();

  display.setTextSize(3);
  display.setCursor(64, 34);
  display.print(from);

  display.setTextSize(4);
  int x = 4;

  if (num < 10) {
    x += 2 * 24;
  } else if (num < 100) {
    x += 1 * 24;
  }

  display.setCursor(x, 4);
  display.print(num);

  if (num >= from) {
    display.drawFastHLine(8, 48, 128 - 16, WHITE);
  }

  display.display();
}

void drawMaxCount(int value) {
  display.setTextSize(4);
  display.clearDisplay();

  display.setCursor(28, 20);
  display.print(value);
  
  display.display();
}

bool modeSetup = false;
int resetButtonPressTime;
int buttonPressDuration;
bool waitResetButton = false;

void loop() {
  b_plus.update();
  b_minus.update();
  b_reset.update();

  if (modeSetup) {
    if (b_plus.fell()) {
      setMaxCount(getMaxCount() + 1);
    }
    if (b_minus.fell()) {
      setMaxCount(getMaxCount() - 1);
    }
    if (b_reset.fell()) {
      modeSetup = false;
      drawCurrent(getCounter(), getMaxCount());
      resetButtonPressTime = millis();
    }


  } else {    

    
    if (b_plus.fell()) {
      setCounter(getCounter() + 1);
    }
    if (b_minus.fell()) {
      setCounter(getCounter() - 1);
    }
    
    if (b_reset.fell()) {
      waitResetButton = true;
      resetButtonPressTime = millis();
    }
    if (b_reset.rose()) {
      buttonPressDuration = millis() - resetButtonPressTime;
      waitResetButton = false;
    }
    if (waitResetButton && millis() - resetButtonPressTime > 1000) {
      modeSetup = true;
      buttonPressDuration = 0;
      drawMaxCount(getMaxCount());
    } else if (buttonPressDuration > 0) {
      setCounter(0);
      buttonPressDuration = 0;
    }

 
  }
}

void setCounter(int newValue) {
  if (newValue >= 0) {
    EEPROM.write(counter_addr, newValue);
    drawCurrent(newValue, getMaxCount());
  }
}

int getCounter() {
  return EEPROM.read(counter_addr);
}

void setMaxCount(int newValue) {
if (newValue > 0) {
    EEPROM.write(max_count_addr, newValue);
    drawMaxCount(newValue);
  }
}

int getMaxCount() {
  return EEPROM.read(max_count_addr);
}