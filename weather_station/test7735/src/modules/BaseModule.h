#ifndef BASEMODULE_H
#define BASEMODULE_H

#include <Adafruit_ST7735.h>

class BaseModule {
  private:
  Adafruit_ST7735 *_tft;
  int8 originX, originY;

  public:
  BaseModule(Adafruit_ST7735 *tft, int8 originX, int8 originY ) {
    _tft = tft;
    this->originX = originX;
    this->originY = originY;
  }
   
  void testDraw();
};

#endif