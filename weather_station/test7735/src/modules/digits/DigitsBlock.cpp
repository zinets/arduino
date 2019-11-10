#include "DigitsBlock.h"


void DigitsBlock::draw() {
    _tft->drawRect(originX, originY, 48, 48, ST77XX_WHITE);

    _tft->setTextSize(3);
    _tft->print(_value);
}