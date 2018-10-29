#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#include "gpsData.h"

class GpsSensor {
private:
  TinyGPSPlus *gps;
  Stream *stream;
  bool smartDelay(unsigned long ms);
public:
  GpsSensor(int rxPin, int txPin, int gpsSpeed);
  GpsSensor(HardwareSerial *serial);

  GpsData currentGpsData;
  bool updateGpsData();
};
