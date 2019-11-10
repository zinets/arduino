#ifndef DUMMY_H
#define DUMMY_H

#include "../BaseScreen.h"
#include "../../modules/BaseModule.h"

class DummyScreen: public BaseScreen {
    private:
        BaseModule *blocks[3];

    public:
        DummyScreen(Adafruit_ST7735 *tft): BaseScreen(tft) {
            for (int x = 0; x < 3; x++) {
                BaseModule *block = new BaseModule(_tft, 8 + 48 * x, 40);
                blocks[x] = block;
            }
        };
        void update();
};

#endif