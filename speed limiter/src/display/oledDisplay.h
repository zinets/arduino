// oled - 13, 135
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "..\gpsData.h"

class OledDisplay {
private:
    Adafruit_SSD1306 *display;
    unsigned long nextTimeToUpdate;
    bool countrySideDriving;

    void updateGpsData();
    void displayStateNoSats();
    void displayStateNoSpeed(int numOfSats);   
    void displayStateCitySpeed(float speed);
    void displayStateCountrySideSpeed(float speed);
    void displayStateSpeedOfLight(float speed);

    void displayCityMode();
public:
    OledDisplay();  
    GpsData gpsData;
    bool speedLimitReached;

    void changeDrivingMode();
    void update(unsigned long millis);
    bool overSpeed(); // did reach speed limit?
};