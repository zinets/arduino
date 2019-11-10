#ifndef DUMMY_H
#define DUMMY_H

#include "../BaseScreen.h"
#include "../../modules/BaseModule.h"

class DummyScreen: public BaseScreen {
    private:
        BaseModule *blocks[3];

    public:
        DummyScreen(Adafruit_ST7735 *tft): BaseScreen(tft) {
            configureBlocks();
        };
        void update();
        void configureBlocks();
};

#endif