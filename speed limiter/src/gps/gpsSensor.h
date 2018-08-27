#include <Arduino.h>
#include <TinyGPS++.h>

class GpsSensor {
private:
  TinyGPSPlus gps;
public:
  GpsSensor();
  float currentSpeed();
};
