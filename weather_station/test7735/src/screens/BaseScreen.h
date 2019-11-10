#ifndef BASESCREEN_H
#define BASESCREEN_H

#include <Adafruit_ST7735.h>
#include "../modules/BaseModule.h"

class BaseScreen {
    protected:
        Adafruit_ST7735 *_tft;
        int backgroundColor = ST7735_BLACK;
        
        BaseModule *blocks[3];
    public:
        BaseScreen(Adafruit_ST7735 *tft) {
            _tft = tft;

            configureBlocks();
        }
        
        virtual void configureBlocks();
        virtual void update();
};

#endif