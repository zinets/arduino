#include <Arduino.h>
#include <SoftwareSerial.h>

#include "gps/gpsSensor.h"

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 4800;

SoftwareSerial ss(RXPin, TXPin);
GpsSensor *gpsSensor;

void setup() {
    Serial.begin(115200);
    ss.begin(GPSBaud);

    gpsSensor = new GpsSensor(&ss);
}

void loop() {
    // put your main code here, to run repeatedly:
}
