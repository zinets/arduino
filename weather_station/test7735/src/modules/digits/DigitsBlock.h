#ifndef DIGITS_H
#define DIGITS_H

#include "../BaseModule.h"

enum DigitType {
    DIGIT, GRADUS, PERCENT
};

class DigitsBlock: public BaseModule {
    private:
        int _value;
    protected:
        
    public:
        DigitsBlock(Adafruit_ST7735 *tft, int8 originX, int8 originY): BaseModule(tft, originX, originY) {

        }

        bool fixedWidth = true;
        DigitType digitType = DIGIT;

        void setValue(int value) {
            _value = value;
        }       
        void draw() override;
};

class PercentBlock: public DigitsBlock {
    public:
        PercentBlock(Adafruit_ST7735 *tft, int8 originX, int8 originY): DigitsBlock(tft, originX, originY) {
            digitType = PERCENT;
            fixedWidth = false;
        }
};

#endif