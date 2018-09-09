#include <Arduino.h>

class AudioAlarm {
private:
    int outPin;
public:
    AudioAlarm(int pin);
    void makeNoise();
};