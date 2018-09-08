#include "oledDisplay.h"

typedef enum State {
    StateNoStas, // no sats
    StateNoSpeed, // speed less than 5 kmph
    StateNormalSpeed, // speed for city
    StateOverSpeed1, // speed for country-side
    StateOverSpeed2, // alarm-alarm!
} State;

static float minimumSpeed = 6;
static float maxCitySpeed = 25;
static float maxCountrySideSpeed = 50;

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
    
    display->display(); //output 'display buffer' to screen 
}

void OledDisplay::update(GpsData gpsData) {
    display->clearDisplay();
    // Serial.print("speed (2)"); Serial.println(gpsData.speed);

    // display->setTextSize(3);
    // display->invertDisplay(false);
    // display->setCursor(0, 0);
    // display->println(gpsData.speed);
    // display->display();

    // return;

    if (/*!gpsData.isValid || */gpsData.numberOfSats < 5) {
        displayStateNoSats();      
    } else if (gpsData.speed < minimumSpeed) {
        displayStateNoSpeed(gpsData.numberOfSats);
    } else if (gpsData.speed < maxCitySpeed) {
        // Serial.println("!");
        displayStateCitySpeed(gpsData.speed);
    } else if (gpsData.speed < maxCountrySideSpeed && countrySideDriving) {
        displayStateCountrySideSpeed(gpsData.speed);
    } else {
        displayStateSpeedOfLight(gpsData.speed);
    }
}

void OledDisplay::displayStateNoSats() {
    // нарисовать иконку спутника
    display->setTextSize(3);
    display->setCursor(0, 0);
    display->print("NO SATS!");
    display->display();
}

void OledDisplay::displayStateNoSpeed(int numOfSats) {
    // выести кол-во спутников
    display->setTextSize(3);
    display->setCursor(0, 0);
    display->print("SATS: ");
    display->print(numOfSats);
    display->display();
}

void OledDisplay::displayStateCitySpeed(float speed) {
    // вывести текущую скорость
    display->setTextSize(1);
    display->invertDisplay(false);
    display->setCursor(0, 0);
    display->println(speed);
    display->display();
}

void OledDisplay::displayStateCountrySideSpeed(float speed) {
    displayStateCitySpeed(speed);
}

void OledDisplay::displayStateSpeedOfLight(float speed) {
    // вывести скорость инвертировано
    display->setTextSize(3);
    display->invertDisplay(true);
    display->setCursor(0, 0);
    display->print(speed);
    display->display();
}