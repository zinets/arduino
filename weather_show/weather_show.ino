#include "U8glib.h"

#include "resources.h"

#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13

#define DEGREE_SYMBOL char(176)

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

  static u8 counter = 0;

void setup(void) {
  u8g.setFont(u8g_font_unifont);
  u8g.setFontPosTop();
}

void draw(void) {
  
  u8g.drawStr(0, 1, "Hello");
  u8g.drawHLine(0, 1+14, 40);
  u8g.setScale2x2();            // Scale up all draw procedures
  u8g.drawStr(0, 12, "Hello");      // actual display position is (0,24)
  u8g.drawHLine(0, 12+14, 40);      // All other procedures are also affected
  u8g.undoScale();          // IMPORTANT: Switch back to normal mode
}

void drawCurrentCondition() {
    IconData iconData = icons[counter];
    u8g.drawXBMP(2, (64 - iconData.iconHeight) / 2, iconData.iconWidth, iconData.iconHeight, iconData.iconName);

    String str = "(" + String(00) + "/" + String(99) + String (DEGREE_SYMBOL) + "C)";
    u8g.setPrintPos(56, 40);
    u8g.print(str);
    
    u8g.setScale2x2();
    u8g.drawStr(32, 0, "8 C"); 
    u8g.undoScale();
}

void loop(void) {
  u8g.firstPage();
  do {
    drawCurrentCondition();
    //draw();
  } while ( u8g.nextPage() );

  if (++counter >= ConditionsCount) {
    counter = 0;
  }
  // rebuild the picture after some delay
  delay(1000);
}

