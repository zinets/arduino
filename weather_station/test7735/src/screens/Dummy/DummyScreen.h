#ifndef DUMMY_H
#define DUMMY_H

#include "../BaseScreen.h"
#include "../../modules/BaseModule.h"
#include "../../modules/digits/DigitsBlock.h"
#include "../../modules/digits/TempBlock.h"

class DummyScreen: public BaseScreen {
    private:
        BaseModule *blocks[3];
        DigitsBlock *dBlock;
        TempBlock *dBlock1;
    public:
        DummyScreen(Adafruit_ST7735 *tft): BaseScreen(tft) {
            configureBlocks(); // почему в родительском классе не вызывается этот метод, который я переопределил в этом наследнике??         
        };
        
        void update() override {
            _tft->fillScreen(backgroundColor);

            for (int x = 0; x < 1; x++) {
                BaseModule *block = blocks[x];
                block->draw();
            }              
            dBlock->draw();
            dBlock1->draw();
        }

        void configureBlocks() override;
};

#endif