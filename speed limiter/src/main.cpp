// #include <Arduino.h>
// #include <SoftwareSerial.h>

// oled - 13, 135
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#include "gps/gpsSensor.h"

static const int RXPin = 2, TXPin = 3;
static const uint32_t GPSBaud = 4800;

SoftwareSerial ss(RXPin, TXPin);
GpsSensor *gpsSensor;

#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 OLED(OLED_RESET);

void setup() {
    Serial.begin(9600);
    ss.begin(GPSBaud);

    gpsSensor = new GpsSensor(&ss);

    OLED.begin();
  OLED.clearDisplay();

  //Add stuff into the 'display buffer'
  OLED.setTextWrap(false);
  OLED.setTextSize(1);
  OLED.setTextColor(WHITE);
  OLED.setCursor(0,0);
  OLED.println("automatedhome.party");
 
  OLED.display(); //output 'display buffer' to screen  
  OLED.startscrollleft(0x00, 0x0F); //make display scroll 
}   

void loop() {
    Serial.println("read..");
    
    // gpsSensor->currentSpeed();

    delay(1000);
}
