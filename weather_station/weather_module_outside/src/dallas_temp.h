#include <Arduino.h>

class DallasSensor {
private:
  int pin;
  float temperature;
public:
  DallasSensor(int sensorPin);
  bool update();
  float getTemperature();
};
