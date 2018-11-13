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
    // lcd pins
    // rs, en, d0, d1, d2, d3
    #define LCD_PINS 12, 11, 9, 8, 7, 6
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
    // lcd pins
    //Define 74HC595 Connections
    const int Clock = D5;
    const int Data = D6;
    const int Latch = D7;

    #define LCD_PINS D5, D6, D7
#endif

#endif
