#include <Arduino.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

class GpsSensor {
private:
  TinyGPSPlus gps;
  SoftwareSerial *ss;
public:
  GpsSensor(SoftwareSerial *serialPort);
  float currentSpeed();
};
