#include "display/oledDisplay.h"
#include "gps/gpsSensor.h"
#include "audioAlarm.h"

#ifdef DEBUG
// #include <ArduinoLog.h>  // id: 1532
#endif
#include <Bounce2.h>     // id: 1106

#ifdef TARGET_ESP8266
#warning BUILDING for esp
static const int RXPin = 12, TXPin = 13; // D6, D7
static const uint32_t GPSBaud = 9600;
static const int audioAlarmPin = 14;  // D5
static const int buttonPin = 16;      // D0

#include <SoftwareSerial.h>
#endif

#ifdef TARGET_AVR
#warning BUILDING for avr
static const int RXPin = 2, TXPin = 3;
static const uint32_t GPSBaud = 9600;
static const int audioAlarmPin = 12;
static const int buttonPin = 11;
#endif

GpsSensor *gpsSensor;
OledDisplay *oledDisplay;
AudioAlarm alarm(audioAlarmPin);
Bounce debouncer = Bounce(); 

void setup() {

#ifdef TARGET_ESP8266
  Serial.begin(9600);

  SoftwareSerial *ss = new SoftwareSerial(RXPin, TXPin);
  ss->begin(GPSBaud);
  gpsSensor = new GpsSensor(ss);

  #ifdef DEBUG
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);
  #endif

#else
  Serial.begin(GPSBaud);
  gpsSensor = new GpsSensor(&Serial);
#endif
    
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
      oledDisplay->changeDrivingMode();

      alarm.makeTripleBeepNoise();
    }
  
    alarm.update(ms);
    oledDisplay->update(ms);
}
