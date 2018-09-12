#include "display/oledDisplay.h"
#include "gps/gpsSensor.h"
#include "audioAlarm.h"

#include <ArduinoLog.h>
#include <Bounce2.h>

static const int RXPin = 12, TXPin = 13;
static const uint32_t GPSBaud = 9600;
static const int audioAlarmPin = 14;
static const int buttonPin = 16;

GpsSensor *gpsSensor;
OledDisplay *oledDisplay;
AudioAlarm alarm(audioAlarmPin);
Bounce debouncer = Bounce(); 

void setup() {
  Serial.begin(9600);
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);

  gpsSensor = new GpsSensor(RXPin, TXPin, GPSBaud);
  oledDisplay = new OledDisplay(); 

  // debouncer.attach(buttonPin, INPUT_PULLUP);
  // debouncer.interval(50);  

  alarm.makeNoise();
}   

void loop() {
  gpsSensor->updateGpsData();
  oledDisplay->update(gpsSensor->currentGpsData);
  
  debouncer.update();
  
  // if (debouncer.fell()) {
  //   Log.notice("pressed!");
  //   oledDisplay->countrySideDriving = !oledDisplay->countrySideDriving;
  // }

  gpsSensor->smartDelay(1000);
}
