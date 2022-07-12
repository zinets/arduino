#include <Arduino.h>

#include <Adafruit_SH1106.h>
#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);

#include <EncButton.h>
EncButton<EB_TICK, 5, 6, 7> enc;  // энкодер с кнопкой <CLK, DT, SW>

#include <EEPROM.h>
const int counter_addr = 1;
volatile int counter = 50;
const int cooler_state_addr = 2;
bool coolerOff = false;

int fanPin = 9;

void updateDisplay() {
  display.setTextSize(4);
  display.clearDisplay();

  display.setCursor(28, 20);
  if (coolerOff) {
    display.print("Off");
  } else {
    display.print(counter);
  }
  
  display.display();
}

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}

// читать "установить выключенное состояние".. дурацкое имя, но пусть уже будет, работает
void setCoolerState(bool state) {
  coolerOff = state;
  EEPROM.write(cooler_state_addr, state);
}

void incCounter() {
  if (counter < 100) {
    counter += 2;

    EEPROM.write(counter_addr, counter);
  } else {
    counter = 100;
  }
  setCoolerState(false);
}

void decCounter() {
  if (counter > 50) {
    counter -= 2;

    EEPROM.write(counter_addr, counter);
  } else {
    counter = 50;
  }
  setCoolerState(false);
}


void setup() {
  pinMode(fanPin, OUTPUT);

  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  
  display.setRotation(2);
  display.setTextColor(WHITE);
  
  Serial.begin(9600);

  counter = max(50, min(100, EEPROM.read(counter_addr)));
  coolerOff = EEPROM.read(cooler_state_addr);
  updateDisplay();

  setPwmFrequency(fanPin, 1);
}

void loop() {
  enc.tick();

  if (enc.click()) {    
    setCoolerState(!coolerOff);
    updateDisplay();
  } else if (enc.left()) {
    incCounter();
  } else if (enc.right()) {
    decCounter();
  }

  if (enc.turn()) {
  //   Serial.println(counter);

    updateDisplay();
  }

  int i = coolerOff ? 0 : map(counter, 50, 100, 150, 255);
  analogWrite(fanPin, i);  

}