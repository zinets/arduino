#include "audioAlarm.h"

static int noise1[4] = {100, 50, 100, 0}; // double "beep"
static int startNoise[2] = {200, 0}; // short start "beep"
static int alarmNoise[6] = {100, 50, 100, 50, 300, 0}; // "alarm-alarm" beep

AudioAlarm::AudioAlarm(int pin) {
    outPin = pin;
    pinMode(outPin, OUTPUT);
}

void AudioAlarm::makeStartNoise() {
    makeNoise(&startNoise[0], 2);
}

void AudioAlarm::makeDoubleBeepNoise() {
    makeNoise(&noise1[0], 4);
}

void AudioAlarm::makeTripleBeepNoise() {
    makeNoise(&alarmNoise[0], 6);
}

void AudioAlarm::makeNoise(int *p, int pLen) {
    if (buzy) {
        return;
    }
    
    nextMillis = millis();
    index = 0; 
    buzy = true;

    pattern = p;
    patternLength = pLen;  

    update(nextMillis); 
}

void AudioAlarm::update(int millis) {
    if (!buzy) {
        return;
    }

    if (millis >= nextMillis) {
        digitalWrite(outPin, index % 2 == 0 ? HIGH : LOW);

        nextMillis = millis + pattern[index];
        if (++index >= patternLength) {
            buzy = false;
        } 
    }
}