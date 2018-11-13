#ifndef HARDWARE_H
#define HARDWARE_H

#include <Arduino.h>

// avr connection
#ifdef TARGET_AVR
    // button
    #define BUTTON_PIN    3
    // leds
    #define NUM_LEDS 1
    #define DATA_PIN 4
    // eeprom addr
    #define EEPROM_ADDR   0
    // one wire sensor
    #define ONE_WIRE_BUS 5
#endif

// esp connection
#ifdef TARGET_ESP8266

    // button
    #define BUTTON_PIN    D2
    // leds
    #define NUM_LEDS 1
    #define DATA_PIN 4
    // eeprom addr
    #define EEPROM_ADDR   0
    // one wire sensor
    #define ONE_WIRE_BUS 5
#endif

#endif