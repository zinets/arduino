#include <TinyGPS++.h>
#include "gpsData.h"

class GpsSensor {
private:
  TinyGPSPlus *gps;
  Stream *stream;
  bool smartDelay(unsigned long ms);
public:
  GpsSensor(Stream *serial);

  GpsData currentGpsData;
  bool updateGpsData();
};
