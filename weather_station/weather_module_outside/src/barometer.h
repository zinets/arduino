#include <Arduino.h>

class Barometer {
private:
  float pressure;
  float temperature;
public:
  bool update();
  float getPressure();
  float getTemperature();
};
