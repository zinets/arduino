#include "BaseScreen.h"

void BaseScreen::update() {
    _tft->fillScreen(backgroundColor);

    for (int x = 0; x < 3; x++) {
        BaseModule *block = blocks[x];
        block->draw();
    }              
}

void BaseScreen::configureBlocks() {

}