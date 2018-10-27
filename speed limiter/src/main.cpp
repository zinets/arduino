#include "display/oledDisplay.h"
#include "gps/gpsSensor.h"
#include "audioAlarm.h"

#ifdef DEBUG
#include <ArduinoLog.h>  // id: 1532
#endif
#include <Bounce2.h>     // id: 1106

#ifdef TARGET_ESP8266
#warning BUILDING for esp
static const int RXPin = 12, TXPin = 13;
static const uint32_t GPSBaud = 9600;
static const int audioAlarmPin = 14;  // D5
static const int buttonPin = 16;      // D0
#endif

#ifdef TARGET_AVR
#warning BUILDING for avr
static const int RXPin = 12, TXPin = 13;
static const uint32_t GPSBaud = 9600;
static const int audioAlarmPin = 12;
static const int buttonPin = 11;
#endif

GpsSensor *gpsSensor;
OledDisplay *oledDisplay;
AudioAlarm alarm(audioAlarmPin);
Bounce debouncer = Bounce(); 

void setup() {
  Serial.begin(9600);

  #ifdef DEBUG
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);
  #endif

  // gpsSensor = new GpsSensor(RXPin, TXPin, GPSBaud);

  oledDisplay = new OledDisplay(); 

  debouncer.attach(buttonPin, INPUT_PULLUP);
  debouncer.interval(50);  

  alarm.makeStartNoise();
}   


void loop() {
    unsigned long ms = millis();

    // if (gpsSensor->updateGpsData()) {
    //   oledDisplay->gpsData = gpsSensor->currentGpsData;
       
    // }
    
    debouncer.update();    
    if (debouncer.fell()) {
      Log.notice("pressed!");
      oledDisplay->changeDrivingMode();

      alarm.makeTripleBeepNoise();
    }
  
    alarm.update(ms);
    oledDisplay->update(ms);
}
