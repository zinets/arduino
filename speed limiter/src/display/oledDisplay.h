// oled - 13, 135
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "..\gpsData.h"

class OledDisplay {
private:
    Adafruit_SSD1306 *display;
public:
    OledDisplay();  
    void update(GpsData gpsData);
};