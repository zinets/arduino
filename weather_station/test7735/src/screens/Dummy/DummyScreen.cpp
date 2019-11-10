#include "DummyScreen.h"

void DummyScreen::configureBlocks() {
    for (int x = 0; x < 2; x++) {
        BaseModule *block = new BaseModule(_tft, 8 + 48 * x, 40);
        blocks[x] = block;
    }
    dBlock = new DigitsBlock(_tft, 8 + 48 * 2, 40);   
    dBlock->setValue(23);
}