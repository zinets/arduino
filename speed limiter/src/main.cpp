#include "display/oledDisplay.h"
#include "gps/gpsSensor.h"

static const int RXPin = 12, TXPin = 13;
static const uint32_t GPSBaud = 9600;
GpsSensor *gpsSensor;
OledDisplay *oledDisplay;

void setup() {
  Serial.begin(9600);
  gpsSensor = new GpsSensor(RXPin, TXPin, GPSBaud);
  oledDisplay = new OledDisplay(); 
}   

void loop() {
  gpsSensor->updateGpsData();
  oledDisplay->update(gpsSensor->currentGpsData);
  
}
