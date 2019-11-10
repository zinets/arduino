#include "DummyScreen.h"

// DummyScreen::DummyScreen(Adafruit_ST7735 *tft) {
//     _tft = tft;    
// }

void DummyScreen::configureBlocks() {
    Serial.println("configureBlocks in");
    for (int x = 0; x < 3; x++) {
        BaseModule *block = new BaseModule(_tft, 8 + 48 * x, 40);
        blocks[x] = block;
    }

Serial.println("configureBlocks out");
}

void DummyScreen::update() {
    for (int x = 0; x < 3; x++) {
        BaseModule *block = blocks[x];
        block->draw();
    }       
}