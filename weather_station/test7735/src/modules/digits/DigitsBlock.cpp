#include "DigitsBlock.h"

void DigitsBlock::setValue(int value) {

}

void DigitsBlock::draw() {
    _tft->fillRect(originX, originY, 48, 48, ST77XX_YELLOW);
    _tft->drawRect(originX, originY, 48, 48, ST77XX_WHITE);
}