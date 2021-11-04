#include <Arduino.h>
#include "../digits/digits.h"

#define DELAY 1500

void setup() {
  setupLed();
}

void loop() {
  for (int i = 0; i < 10; i++) {
    shownum(i);
  
    delay(DELAY);
  }
}