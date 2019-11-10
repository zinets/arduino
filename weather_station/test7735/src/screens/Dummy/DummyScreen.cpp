#include "DummyScreen.h"

void DummyScreen::configureBlocks() {
    for (int x = 0; x < 1; x++) {
        BaseModule *block = new BaseModule(_tft, 8 + 48 * x, 40);
        block->isDelimiterVisible = true;
        blocks[x] = block;
    }
    dBlock = new DigitsBlock(_tft, 8 + 48 * 1, 40);   
    dBlock->fixedWidth = false;
    dBlock->isDelimiterVisible = true;
    dBlock->setValue(-4);
    
    dBlock1 = new TempBlock(_tft, 8 + 48 * 2, 40);   
    dBlock1->fixedWidth = false;
    dBlock1->setValue(-64);
}