#include "DummyScreen.h"

void DummyScreen::configureBlocks() {
    BaseModule *block = new BaseModule(_tft, 8 + 48 * 0, 40);
    block->isDelimiterVisible = true;
    blocks[0] = block;

    PercentBlock *dBlock = new PercentBlock(_tft, 8 + 48 * 1, 40);   
    blocks[1] = dBlock;
    dBlock->isDelimiterVisible = true;
    dBlock->setValue(-4);
    
    TemperatureBlock *dBlock1 = new TemperatureBlock(_tft, 8 + 48 * 2, 40);   
    blocks[2] = dBlock1;
    dBlock1->setValue(-64);
}