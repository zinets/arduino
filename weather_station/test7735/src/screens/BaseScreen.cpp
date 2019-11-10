#include "BaseScreen.h"

BaseScreen::BaseScreen(Adafruit_ST7735 *tft) {
    _tft = tft;    
    Serial.println("BaseScreen init");
    configureBlocks();
    Serial.println("BaseScreen configured");
}

void BaseScreen::update() {

}

void BaseScreen::configureBlocks() {

}