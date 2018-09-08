#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#include "gpsData.h"

class GpsSensor {
private:
  TinyGPSPlus *gps;
  SoftwareSerial *ss;
public:
  GpsSensor(int rxPin, int txPin, int gpsSpeed);

  GpsData currentGpsData;
  void updateGpsData();
};
