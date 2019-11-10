#include "BaseModule.h"

void BaseModule::testDraw() {
    _tft->fillRect(originX, originY, 48, 48, ST77XX_BLUE);
    _tft->drawRect(originX, originY, 48, 48, ST77XX_RED);

}