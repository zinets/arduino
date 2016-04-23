#include <Arduino.h>

#include "U8glib.h"

#include "resources.h"

#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13

#define DEGREE_SYMBOL       char(176)
#define MARKER_SEPARATOR    ';'
#define MARKER_TEMP         't'
#define MARKER_FORECAST_TEMP 'f'

U8GLIB_SSD1306_128X64 u8g(OLED_CLK, OLED_MOSI, OLED_CS, OLED_DC, OLED_RESET);
typedef enum Conditions { 
  ClearDay, 
  ClearNight,
  Rain,
  Snow,
  Sleet,
  Wind,
  Fog,
  Cloudy,
  PartlyCloudyDay, 
  PartlyCloudyNight,
  ConditionsCount,
} Condition;

typedef struct IconData {
  u8 iconWidth;
  u8 iconHeight;
  unsigned char *iconName;
} IconData;

IconData icons[ConditionsCount] = {
  {iconWidth: clear_day_width, iconHeight: clear_day_height, iconName: clear_day_bits}, // ClearDay, 
  {iconWidth: clear_night_width, iconHeight: clear_night_height, iconName: clear_night_bits}, // ClearNight,
  {iconWidth: rain_width, iconHeight: rain_height, iconName: rain_bits}, // Rain,
  {iconWidth: snow_width, iconHeight: snow_height, iconName: snow_bits}, // Snow,
  {iconWidth: sleet_width, iconHeight: sleet_height, iconName: sleet_bits}, // Sleet,
  {iconWidth: wind_width, iconHeight: wind_height, iconName: wind_bits}, // Wind,
  {iconWidth: fog_width, iconHeight: fog_height, iconName: fog_bits}, // Fog,
  {iconWidth: cloudy_width, iconHeight: cloudy_height, iconName: cloudy_bits}, // Cloudy,
  {iconWidth: partly_cloudy_day_width, iconHeight: partly_cloudy_day_height, iconName: partly_cloudy_day_bits}, // PartlyCloudyDay, 
  {iconWidth: partly_cloudy_night_width, iconHeight: partly_cloudy_night_height, iconName: partly_cloudy_night_bits}, // PartlyCloudyNight,
};

String currentTemp = "12";
String tomorrowTemp = "20";
Condition currentCondition;
int secondsCount = 0;

typedef enum State {
  StateStarting,
  StateCurrentCondition,
  StateTommorowCondition,
  StateCount,
} State;

u8 currentState = StateStarting;

void setup(void) {
  u8g.setFont(u8g_font_unifont);
  u8g.setFontPosTop();

  Serial.begin(115200);
  Serial.println("Inited");

  currentCondition = Rain;
}

void drawStartLogo() {
  u8g.drawXBMP((128 - WiFi_Logo_width) / 2, (64 - WiFi_Logo_height) / 2 - 3, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
  const u8 r = 4; // radius
  const u8 d = 15; // space between dots
  const u8 startX = 64 - d;
  const u8 startY = 58;
#define DOTS_COUNT 3
  for (int i = 0; i < DOTS_COUNT; i++) {
    if (secondsCount % DOTS_COUNT != i) {
      u8g.drawCircle(startX + d * i, startY, r);
    } else {
      u8g.drawDisc(startX + d * i, startY, r);
    }
  }
}

void drawCurrentCondition() {
    IconData iconData = icons[currentCondition];
    u8g.drawXBMP(2, (64 - iconData.iconHeight) / 2, iconData.iconWidth, iconData.iconHeight, iconData.iconName);

    u8g.setScale2x2();
    u8g.setPrintPos(32, 10);
    String str = currentTemp + String (DEGREE_SYMBOL) + "C"; 
    u8g.print(str);
    u8g.undoScale();
}

void drawTomorrowCondition() {
  IconData iconData = icons[ClearDay];
    u8g.drawXBMP(2, (64 - iconData.iconHeight) / 2, iconData.iconWidth, iconData.iconHeight, iconData.iconName);
    
    u8g.drawStr(56, 10, "Tomorrow");
    u8g.setPrintPos(62, 30);
    String str = tomorrowTemp + String (DEGREE_SYMBOL) + "C"; 
    u8g.print(str);
}

void readSerial() {  
  typedef enum SerialState {
    SerialStateReady,
    SerialStateReadingTemperature,
    SerialStateReadingFTemperature,
  } SerialState;

  String res = "";
  SerialState serialState = SerialStateReady;
  while (Serial.available()) {
    u8 ch = Serial.read();
    
    switch(serialState) {
      case SerialStateReady: {
        switch (ch) {
          case MARKER_TEMP:
            serialState = SerialStateReadingTemperature;
            break;
          case MARKER_FORECAST_TEMP:
            serialState = SerialStateReadingFTemperature;
            break;
          default:
            break;
        }
      } break;
      case SerialStateReadingTemperature: {
        if (ch == MARKER_SEPARATOR) {
          currentTemp = res;
          res = "";
          serialState = SerialStateReady;
        } else {
          res = res + char(ch);          
        }
      } break;
      case SerialStateReadingFTemperature: {
        if (ch == MARKER_SEPARATOR) {
          tomorrowTemp = res;
          res = "";
          serialState = SerialStateReady;
        } else {
          res = res + char(ch);          
        }
      } break;
      default:
        break;
    }
  }  
}

void loop(void) {
  readSerial();

  u8g.firstPage();
  do {
    switch (currentState) {
      case StateStarting:
        drawStartLogo();
        break;
      case StateCurrentCondition:
        drawCurrentCondition();
        break;
      case StateTommorowCondition:
        drawTomorrowCondition();
        break; 
      default:
        break;
    }
  } while (u8g.nextPage());

  delay(1000);
  secondsCount++;

  switch (currentState) {
    case StateStarting: {
      if (currentTemp.length() > 0) {
        currentState++;
      }
    } break;
    default:
      if (secondsCount % 3 == 0) {
        currentState++;
      }
      break;
  }

  if (currentState >= StateCount) {
    currentState = StateCurrentCondition;
  }
}

