#ifndef BASEMODULE_H
#define BASEMODULE_H

#include <Adafruit_ST7735.h>

class BaseModule {
  protected:
    Adafruit_ST7735 *_tft;
    int8 originX, originY;

  public:
    BaseModule(Adafruit_ST7735 *tft, int8 originX, int8 originY) {
      _tft = tft;
      this->originX = originX;
      this->originY = originY;
    }

    /// разделитель - вертикальная линия справа
    bool isDelimiterVisible = false;   
    virtual void draw();
};

#endif