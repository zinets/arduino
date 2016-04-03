#include "U8glib.h"

#include "resources.h"

#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13

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

void draw(void) {
  u8g.drawXBMP(12, 12, clear_day_width, clear_day_height, clear_day_bits);
}

void drawPage1(void) {
  u8g.drawXBMP(12, 12, clear_day_width, clear_day_height, clear_day_bits);
}

void drawPage2(void) {
  u8g.drawXBMP(12, (64 - WiFi_Logo_height) / 2, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}

void setup(void) {
}

void loop(void) {
  static counter = 0;
  u8g.firstPage();
  do {
    IconData iconData = icons[counter++];
    u8g.drawXBMP(12, (64 - iconData.iconHeight) / 2, iconData.iconWidth, iconData.iconHeight, iconData.iconName);
  } while ( u8g.nextPage() );

  if (counter > ConditionsCount) {
    counter = 0;
  }
  // rebuild the picture after some delay
  delay(1000);
}

