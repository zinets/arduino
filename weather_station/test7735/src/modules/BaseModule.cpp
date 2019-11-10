#include "BaseModule.h"

void BaseModule::draw() {
    if (isDelimiterVisible) {
        _tft->drawLine(originX + 48, originY, originX + 48, originY + 48, ST7735_WHITE);
    }
}