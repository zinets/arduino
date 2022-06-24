#include <Arduino.h>
#include <EEPROM.h>

#include <Adafruit_SH1106.h>
#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);

#include <Bounce2.h>
#define BOUNCE_PIN_PLUS   2
#define BOUNCE_PIN_MINUS  3
#define BOUNCE_PIN_RESET  11

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
  display.setTextSize(2);

  // setCounter(0);
  // setMaxCount(130);
  drawCurrent(getCounter(), getMaxCount());
}

void drawCurrent(int num, int from) {
  display.clearDisplay();

  display.setCursor(4, 4);
  display.print(num);

  display.setCursor(64, 4 + 8 * 2 + 4);
  display.print(from);

  display.display();
}

void loop() {
  
  b_plus.update();
  if (b_plus.fell()) {
    setCounter(getCounter() + 1);
  }

  b_minus.update();
  if (b_minus.fell()) {
    setCounter(getCounter() - 1);
  }

  b_reset.update();
  if (b_reset.fell()) {
    setCounter(0);
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
    drawCurrent(getCounter(), newValue);
  }
}

int getMaxCount() {
  return EEPROM.read(max_count_addr);
}