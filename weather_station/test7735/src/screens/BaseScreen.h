#ifndef BASESCREEN_H
#define BASESCREEN_H

#include <Adafruit_ST7735.h>

class BaseScreen {
    protected:
    Adafruit_ST7735 *_tft;
    
    public:
    BaseScreen(Adafruit_ST7735 *tft) {
        _tft = tft;
    }
    virtual void update();
};

#endif