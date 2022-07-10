#include <Arduino.h>

#include <Adafruit_SH1106.h>
#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);

#include <EncButton.h>
EncButton<EB_TICK, 5, 6, 7> enc;  // энкодер с кнопкой <CLK, DT, SW>

#include <EEPROM.h>
const int counter_addr = 22;
volatile int counter = 50;

void updateDisplay() {
  display.setTextSize(4);
  display.clearDisplay();

  display.setCursor(28, 20);
  display.print(counter);
  
  display.display();
}

void setup() {
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  
  display.setRotation(2);
  display.setTextColor(WHITE);
  
  Serial.begin(9600);

  counter = EEPROM.read(counter_addr) < 255 ? : 50;
  updateDisplay();
}

void incCounter() {
  if (counter < 100) {
    counter++;

    EEPROM.write(counter_addr, counter);
  } else {
    counter = 100;
  }
}

void decCounter() {
  if (counter > 50) {
    counter--;

    EEPROM.write(counter_addr, counter);
  } else {
    counter = 50;
  }
}

void loop() {
  enc.tick();

  if (enc.left()) {
    incCounter();
  } else if (enc.right()) {
    decCounter();
  }

  if (enc.turn()) {
  //   Serial.println(counter);

    updateDisplay();
  }
}