#include "audioAlarm.h"

AudioAlarm::AudioAlarm(int pin) {
    outPin = pin;
    pinMode(outPin, OUTPUT);
}

void AudioAlarm::makeNoise() {
    digitalWrite(outPin, HIGH);
    delay(200);
    digitalWrite(outPin, LOW);
}