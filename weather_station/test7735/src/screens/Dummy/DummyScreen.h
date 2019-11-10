#ifndef DUMMY_H
#define DUMMY_H

#include "../BaseScreen.h"
#include "../../modules/digits/DigitsBlock.h"

class DummyScreen: public BaseScreen {
    private:
        
    public:
        DummyScreen(Adafruit_ST7735 *tft): BaseScreen(tft) {
            configureBlocks(); // почему в родительском классе не вызывается этот метод, который я переопределил в этом наследнике??         
        };

        virtual void configureBlocks() override;
};

#endif