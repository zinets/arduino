#ifndef HAL_H
#define HAL_H
#include <Arduino.h>

#ifdef TARGET_ESP8266

    // button
    #define BUTTON_PIN    D8
    // leds
    #define LED_TYPE      WS2812
    #define COLOR_ORDER   GRB
    #define NUM_LEDS      1
    #define DATA_PIN      D4
    // eeprom addr
    #define EEPROM_ADDR   0
    // one wire sensor
    #define ONE_WIRE_BUS  5
    // lcd pins
    //Define 74HC595 Connections
    const int Clock = D5;
    const int Data = D6;
    const int Latch = D7;

    #define LCD_PINS D5, D6, D7
#endif


#endif
