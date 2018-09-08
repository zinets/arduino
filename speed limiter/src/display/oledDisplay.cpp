#include "oledDisplay.h"

#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

OledDisplay::OledDisplay() {
    display = new Adafruit_SSD1306(OLED_RESET);
    
    display->begin();
    display->clearDisplay();

    //Add stuff into the 'display buffer'
    display->setTextWrap(false);
    display->setTextSize(2);
    display->setTextColor(WHITE);
    display->setCursor(0,0);
    display->println("ready");
    
    display->display(); //output 'display buffer' to screen 
}

void OledDisplay::update(GpsData gpsData) {
    display->clearDisplay();
    display->setCursor(0, 0);

    if (gpsData.isValid)  {
        display->print("lat: ");
        display->println(gpsData.lat);
        display->print("lon: ");
        display->println(gpsData.lon);
        display->print("spd: ");
        display->println(gpsData.speed);    
    } else {
        display->println("lat: ??");
        display->println("lon: ??");
        display->println("spd: --");
    }
    display->print("sats: ");
    display->println(gpsData.numberOfSats);
    display->invertDisplay(gpsData.isValid);
    display->display();
}