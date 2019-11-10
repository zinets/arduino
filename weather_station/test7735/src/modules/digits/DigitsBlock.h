#ifndef DIGITS_H
#define DIGITS_H

#include "../BaseModule.h"

class DigitsBlock: public BaseModule {
    public:
        void setValue(int value);

        DigitsBlock(Adafruit_ST7735 *tft, int8 originX, int8 originY): BaseModule(tft, originX, originY) {

        }

        void draw() override;
};

#endif