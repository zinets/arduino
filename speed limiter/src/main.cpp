// #include <Arduino.h>
// #include <SoftwareSerial.h>

// oled - 13, 135
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#include "gps/gpsSensor.h"

static const int RXPin = 12, TXPin = 13;
static const uint32_t GPSBaud = 9600;

SoftwareSerial ss(RXPin, TXPin);
GpsSensor *gpsSensor;

TinyGPSPlus gps;

#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 OLED(OLED_RESET);

void testdrawrect(void) {
  for (int16_t i=0; i<OLED.height()/2; i+=2) {
    OLED.drawRect(i, i, OLED.width()-2*i, OLED.height()-2*i, WHITE);
    OLED.display();
    delay(1);
  }
}

void setup() {
    Serial.begin(9600);
    ss.begin(GPSBaud);

    // gpsSensor = new GpsSensor(&ss);

    OLED.begin();
    OLED.clearDisplay();

    

  //Add stuff into the 'display buffer'
  OLED.setTextWrap(false);
  OLED.setTextSize(1);
  OLED.setTextColor(WHITE);
  OLED.setCursor(0,0);
  OLED.println("ready");
 
  OLED.display(); //output 'display buffer' to screen  
  // OLED.startscrollleft(0x00, 0x0F); //make display scroll 
}   

void loop() {
  // OLED.setCursor(0,0);
  // OLED.println("reading..");
  // OLED.display();

  while (ss.available() > 0) {
    int readed = ss.read();
    // Serial.print(readed);
    // Serial.print(" ");

    gps.encode(readed);
  }

  OLED.clearDisplay();  
  OLED.setCursor(0, 0);

  if (gps.location.isValid() && gps.location.isUpdated()) {  
    OLED.print("lat: "); 
    OLED.println(gps.location.lat());

    OLED.print("lon: ");
    OLED.println(gps.location.lng());
  } else {
    OLED.println("lat: ?"); 
    OLED.println("lon: ?");    
  }

  if (gps.speed.isValid() && gps.speed.isUpdated()) {
    OLED.print("spd: "); 
    OLED.println(gps.speed.kmph());
  } else {
    OLED.println("spd: --");
  }

  OLED.display();
  delay(10);
  
  

    // Serial.println("read..");
    
    // gpsSensor->currentSpeed();

    // delay(1000);
}
