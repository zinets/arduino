#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#include "gpsData.h"

class GpsSensor {
private:
  TinyGPSPlus *gps;
  SoftwareSerial *ss;
  bool smartDelay(unsigned long ms);
public:
  GpsSensor(int rxPin, int txPin, int gpsSpeed);

  GpsData currentGpsData;
  bool updateGpsData();
};
