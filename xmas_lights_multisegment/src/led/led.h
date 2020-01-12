#ifndef _led_H
#define _led_H

// global
extern bool nightMode;
void setNightMode(bool mode);

void setupLeds();
void handleLights();

#endif