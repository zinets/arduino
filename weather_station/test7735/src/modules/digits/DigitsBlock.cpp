#include "DigitsBlock.h"


void DigitsBlock::draw() {
    _tft->drawRect(originX, originY, 48, 48, ST77XX_WHITE);

    _tft->setTextSize(2);
    String s = String(_value);
    
    if (fixedWidth && _value < 10) {
        s = "0" + s;
    }

    int16_t x, y;
    uint16_t w, h;
    _tft->getTextBounds(s, originX, originY, &x, &y, &w, &h);
    
    x = originX + (48 - w) / 2;
    y = originY + (48 - h) / 2;
    _tft->setCursor(x, y);

    _tft->print(s);

    _tft->drawCircle(x + w + 2, y, 2, ST7735_WHITE);
}