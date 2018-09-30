#include "gpsSensor.h"
#include <ArduinoLog.h>

#undef TEST
#ifdef TEST
const char *gpsStream =
  "$GPRMC,045103.000,A,3014.1984,N,09749.2872,W,0.67,161.46,030913,,,A*7C\r\n"
  "$GPGGA,045104.000,3014.1985,N,09749.2873,W,1,09,1.2,211.6,M,-22.5,M,,0000*62\r\n"
  "$GPRMC,045200.000,A,3014.3820,N,09748.9514,W,36.88,65.02,030913,,,A*77\r\n"
  "$GPGGA,045201.000,3014.3864,N,09748.9411,W,1,10,1.2,200.8,M,-22.5,M,,0000*6C\r\n"
  "$GPRMC,045251.000,A,3014.4275,N,09749.0626,W,0.51,217.94,030913,,,A*7D\r\n"
  "$GPGGA,045252.000,3014.4273,N,09749.0628,W,1,09,1.3,206.9,M,-22.5,M,,0000*6F\r\n";
#endif

GpsSensor::GpsSensor(int rxPin, int txPin, int gpsSpeed) {
  gps = new TinyGPSPlus();
  ss = new SoftwareSerial(rxPin, txPin);
  ss->begin(gpsSpeed);
}

bool GpsSensor::updateGpsData() {
  bool res = smartDelay(0);

  if (res) {
    currentGpsData.numberOfSats = gps->satellites.value();
    currentGpsData.isValid = currentGpsData.numberOfSats > 4;

    Log.notice("sats %d"CR, currentGpsData.numberOfSats);

    if (gps->location.isUpdated()) {
      currentGpsData.lat = gps->location.lat();
      currentGpsData.lon = gps->location.lng();
      Log.notice(" %D, %D"CR, currentGpsData.lat, currentGpsData.lon);
    }
    if (gps->speed.isUpdated()) { 
      currentGpsData.speed = gps->speed.kmph();
      Log.notice(" %D kmph"CR, currentGpsData.speed);
    }  
  }
  return res;
}

bool GpsSensor::smartDelay(unsigned long ms) {
  unsigned long start = millis();
  bool res = false;
  do {
    while (ss->available()) {
      gps->encode(ss->read());
      res = true;
    }
  } while (millis() - start < ms);

  if (millis() > 5000 && gps->charsProcessed() < 10) {
    Log.error(F("No GPS data received: check wiring"));
  }

  return res;
}