#ifndef BASESCREEN_H
#define BASESCREEN_H

#include <Adafruit_ST7735.h>

class BaseScreen {
    protected:
    Adafruit_ST7735 *_tft;
    int backgroundColor = ST7735_BLACK;
    
    public:
    BaseScreen(Adafruit_ST7735 *tft) {
        _tft = tft;

        configureBlocks();
    }
    
    virtual void configureBlocks();
    virtual void update();
};

#endif