#ifndef TEMPBLOCK_H
#define TEMPBLOCK_H

#include "DigitsBlock.h"

class TempBlock: public DigitsBlock {
    public:
        TempBlock(Adafruit_ST7735 *tft, int8 originX, int8 originY): DigitsBlock(tft, originX, originY) {
            isGradSignVisible = true;
        }

        // void draw() override {            
        //     DigitsBlock::draw();
        //     _tft->drawCircle(12 + 2, 12, 2, ST7735_WHITE);
        // }
};

#endif