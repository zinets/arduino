#ifndef DUMMY_H
#define DUMMY_H

#include "../BaseScreen.h"
#include "../../modules/BaseModule.h"
#include "../../modules/digits/DigitsBlock.h"

class DummyScreen: public BaseScreen {
    private:
        BaseModule *blocks[3];
        DigitsBlock *dBlock;
    public:
        DummyScreen(Adafruit_ST7735 *tft): BaseScreen(tft) {
            for (int x = 0; x < 2; x++) {
                BaseModule *block = new BaseModule(_tft, 8 + 48 * x, 40);
                blocks[x] = block;
            }
            dBlock = new DigitsBlock(_tft, 8 + 48 * 2, 40);            
        };

        void update() {
            for (int x = 0; x < 2; x++) {
                BaseModule *block = blocks[x];
                block->draw();
            }              
            dBlock->draw();
        }
};

#endif