#include "display/oledDisplay.h"
#include "gps/gpsSensor.h"
#include "audioAlarm.h"

#include <ArduinoLog.h>

static const int RXPin = 12, TXPin = 13;
static const uint32_t GPSBaud = 9600;
static const int audioAlarmPin = 14;

GpsSensor *gpsSensor;
OledDisplay *oledDisplay;
AudioAlarm alarm(audioAlarmPin);

void setup() {
  Serial.begin(9600);
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);

  gpsSensor = new GpsSensor(RXPin, TXPin, GPSBaud);
  oledDisplay = new OledDisplay(); 

  alarm.makeNoise();
}   

void loop() {
  gpsSensor->updateGpsData();
  oledDisplay->update(gpsSensor->currentGpsData);
  
  gpsSensor->smartDelay(1000);
}
