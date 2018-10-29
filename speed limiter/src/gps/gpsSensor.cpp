#include "gpsSensor.h"
// #ifdef DEBUG
// #include <ArduinoLog.h>
// #endif

GpsSensor::GpsSensor(Stream *serial) {
  gps = new TinyGPSPlus();
  stream = serial;
}

bool GpsSensor::updateGpsData() {
  bool res = smartDelay(0);

  if (res) {
    currentGpsData.numberOfSats = gps->satellites.value();
    currentGpsData.isValid = currentGpsData.numberOfSats > 4;

    // Log.notice("sats %d"CR, currentGpsData.numberOfSats);

    if (gps->location.isUpdated()) {
      currentGpsData.lat = gps->location.lat();
      currentGpsData.lon = gps->location.lng();
      // Log.notice(" %D, %D"CR, currentGpsData.lat, currentGpsData.lon);
    }
    if (gps->speed.isUpdated()) { 
      currentGpsData.speed = gps->speed.kmph();
      // Log.notice(" %D kmph"CR, currentGpsData.speed);
    }  
  }
  return res;
}

bool GpsSensor::smartDelay(unsigned long ms) {
  unsigned long start = millis();
  bool res = false;
  do {
    while (stream->available()) {
      gps->encode(stream->read());
      res = true;
    }
  } while (millis() - start < ms);

  if (millis() > 5000 && gps->charsProcessed() < 10) {
    // Log.error(F("No GPS data received: check wiring"));
  }

  return res;
}