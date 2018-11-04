#include "audioAlarm.h"

static uint8_t noise1[] = {100, 50, 100, 0}; // double "beep"
static uint8_t startNoise[2] = {200, 0}; // short start "beep"
static uint8_t alarmNoise[] = {100, 50, 100, 50, 250, 0}; // "alarm-alarm" beep

AudioAlarm::AudioAlarm(int pin) {
    outPin = pin;
    
    buzy = false;
    nextMillis = 0;

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

void AudioAlarm::makeNoise(uint8_t *p, int pLen) {
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