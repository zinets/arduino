#ifndef _web_control_H
#define _web_control_H

// OTA support
#ifdef TARGET_ESP8266

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#endif

extern ESP8266WebServer server;

void setupWiFi();
#endif