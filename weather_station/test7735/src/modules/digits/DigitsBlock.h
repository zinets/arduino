#ifndef DIGITS_H
#define DIGITS_H

#include "../BaseModule.h"

class DigitsBlock: public BaseModule {
    private:
        int _value;

    public:
        void setValue(int value) {
            _value = value;
        }

        DigitsBlock(Adafruit_ST7735 *tft, int8 originX, int8 originY): BaseModule(tft, originX, originY) {

        }

        void draw() override;
};

#endif