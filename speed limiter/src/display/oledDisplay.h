// oled - 13, 135
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "..\gpsData.h"

class OledDisplay {
private:
    Adafruit_SSD1306 *display;

    void displayStateNoSats();
    void displayStateNoSpeed(int numOfSats);   
    void displayStateCitySpeed(float speed);
    void displayStateCountrySideSpeed(float speed);
    void displayStateSpeedOfLight(float speed);
public:
    OledDisplay();  
    bool countrySideDriving;
    void update(GpsData gpsData);
};