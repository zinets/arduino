#ifndef BASESCREEN_H
#define BASESCREEN_H

#include <Adafruit_ST7735.h>

class BaseScreen {
    protected:
    Adafruit_ST7735 *_tft;
    virtual void configureBlocks();

    public:
    BaseScreen(Adafruit_ST7735 *tft);
    virtual void update();
};

#endif