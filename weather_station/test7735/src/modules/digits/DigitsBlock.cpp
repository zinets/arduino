#include "DigitsBlock.h"


void DigitsBlock::draw() {
    BaseModule::draw();

    _tft->setTextSize(2);
    String s = String(_value);
    
    if (fixedWidth && _value < 10) {
        s = "0" + s;
    }

    switch (digitType) {
        case PERCENT:
            s = s + "%";
            break;
        default:
            break;
    }

    int16_t x, y;
    uint16_t w, h;
    _tft->getTextBounds(s, originX, originY, &x, &y, &w, &h);
    
    x = originX + (48 - w) / 2;
    y = originY + (48 - h) / 2;
    _tft->setCursor(x, y);

    _tft->print(s);
    if (digitType == GRADUS)
        _tft->drawCircle(x + w + 2, y, 2, ST7735_WHITE);
}