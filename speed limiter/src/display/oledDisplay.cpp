#include "oledDisplay.h"
#include "..\images\oledImages.h"

static const int oledReset = 0;

static float minimumSpeed = 6;
static float maxCitySpeed = 25;
static float maxCountrySideSpeed = 50;

static int updatePeriod = 1000; //250; // ms


OledDisplay::OledDisplay() {
    display = new Adafruit_SSD1306(oledReset);
    nextTimeToUpdate = 0;

    gpsData.isValid = false;
    gpsData.speed = 110;
    gpsData.numberOfSats = 8;
    
    display->begin();
    display->clearDisplay();

    display->setTextWrap(false);
    display->setTextSize(2);
    display->setTextColor(WHITE);
    display->setCursor(0,0);

    display->display(); 
}

void OledDisplay::updateGpsData() {
    display->clearDisplay();

    if (!gpsData.isValid) {
        displayStateNoSats();      
    } else if (gpsData.speed < minimumSpeed) {
        displayStateNoSpeed(gpsData.numberOfSats);
    } else if (gpsData.speed < maxCitySpeed) {
        displayStateCitySpeed(gpsData.speed);
    } else if (gpsData.speed < maxCountrySideSpeed && countrySideDriving) {
        displayStateCountrySideSpeed(gpsData.speed);
    } else {
        displayStateSpeedOfLight(gpsData.speed);
    }

    displayCityMode();

    display->display();
}

void OledDisplay::changeDrivingMode() {
    countrySideDriving = !countrySideDriving;
    nextTimeToUpdate = millis() + updatePeriod;

    updateGpsData();
}

void OledDisplay::update(unsigned long millis) {
    if (millis >= nextTimeToUpdate) {
        nextTimeToUpdate = millis + updatePeriod;

        updateGpsData();
    }
}

// default font size 6x8, setTextSize(x) -> 6*x x 8*x

void OledDisplay::displayStateNoSats() {
    // нарисовать иконку спутника
    display->drawBitmap(64, 0, image_data_searchSat, 64, 64, 1);
    display->setCursor(0, 0);
    // display->drawBitmap(128-16, 0, countrySideDriving ? image_data_country : image_data_city, 16, 16, 1);
}

void OledDisplay::displayStateNoSpeed(int numOfSats) {
    // вывести кол-во спутников
    display->drawBitmap(64, 0, image_data_sat, 64, 64, 1);
    display->setTextSize(2);
    // display->setCursor(128 - 12 * (numOfSats > 10 ? 2 : 1) - 3, 3);
    display->setCursor(64, 64 - 16);
    display->print(numOfSats);
}

void OledDisplay::displayStateCitySpeed(float speed) {
    // вывести текущую скорость
    display->setTextSize(3);
    display->invertDisplay(false);
    display->setCursor(0, 16);
    display->println(speed);
}

void OledDisplay::displayStateCountrySideSpeed(float speed) {
    displayStateCitySpeed(speed);
}

void OledDisplay::displayStateSpeedOfLight(float speed) {
    // вывести скорость инвертировано
    display->setTextSize(3);
    display->invertDisplay(true);
    display->setCursor(0, 16);
    display->print(speed);
}

void OledDisplay::displayCityMode() {
    display->drawBitmap(128-16, 0, countrySideDriving ? image_data_country : image_data_city, 16, 16, 1);
}
