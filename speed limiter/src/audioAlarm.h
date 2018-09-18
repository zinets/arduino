#include <Arduino.h>

class AudioAlarm {
private:
    int outPin;
    bool buzy;
    
    int nextMillis;
    int index, patternLength;
    int *pattern;
    void makeNoise(int *pattern, int patternLen);
public:
    AudioAlarm(int pin);
    
    void makeStartNoise();
    void makeDoubleBeepNoise();
    void makeTripleBeepNoise();
    void update();
};