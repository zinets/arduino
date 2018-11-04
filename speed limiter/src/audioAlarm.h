#include <Arduino.h>

class AudioAlarm {
private:
    int outPin;
    bool buzy;
    
    int nextMillis;
    
    int index, patternLength;
    uint8_t *pattern;
    void makeNoise(uint8_t *pattern, int patternLen);
public:
    AudioAlarm(int pin);
    
    void makeStartNoise(); // single "beep" for start
    void makeDoubleBeepNoise(); // 2 beeps after mode change
    void makeTripleBeepNoise(); // 3 beeps for overspeed warning

    void update(int millis);
};