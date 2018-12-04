#include <Arduino.h>

// !! #define BMP280_ADDRESS                (0x76)

class Barometer {
private:
  float pressure;
  float temperature;
public:
  bool update();
  float getPressure();
  float getTemperature();
};
