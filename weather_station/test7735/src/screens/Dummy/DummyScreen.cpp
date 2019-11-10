#include "DummyScreen.h"

void DummyScreen::update() {
    for (int x = 0; x < 3; x++) {
        BaseModule *block = blocks[x];
        block->draw();
    }       
}