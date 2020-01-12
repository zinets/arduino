#ifndef _led_H
#define _led_H

// global
extern bool nightMode;
void setNightMode(bool mode);

extern bool isGlitterEnabled;
void setGlitterEnabled(bool enabled);

void setupLeds();
void handleLights();

#endif