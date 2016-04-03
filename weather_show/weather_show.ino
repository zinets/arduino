#include "U8glib.h"

#include "resources.h"

#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13

#define DEGREE_SYMBOL char(176)
#define MARKER_TEMP 't'

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

u8 currentTemp;
u8 minTemp;
u8 maxTemp; 
Condition currentCondition;
int secondsCount = 0;

typedef enum State {
  StateStarting,
} State;

State currentState = StateStarting;

void setup(void) {
  u8g.setFont(u8g_font_unifont);
  u8g.setFontPosTop();

  Serial.begin(115200);
  Serial.println("Inited");

  currentCondition = Rain;
  currentTemp = 12;
  minTemp = 9;
  maxTemp = 20;

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
  Serial.print("drawLogo ");
  Serial.println(secondsCount);
}

void drawCurrentCondition() {
    IconData iconData = icons[currentCondition];
    u8g.drawXBMP(2, (64 - iconData.iconHeight) / 2, iconData.iconWidth, iconData.iconHeight, iconData.iconName);

    String str = "(" + String(minTemp) + "/" + String(maxTemp) + " " + String (DEGREE_SYMBOL) + "C)";
    u8g.setPrintPos(56, 40);
    u8g.print(str);
    
    u8g.setScale2x2();
    u8g.setPrintPos(32, 0);
    str = String (currentTemp) + " C)"; 
    u8g.print(str);
    u8g.undoScale();
}

void readSerial() {
  enum State {

  } State;
}

void loop(void) {
  u8g.firstPage();
  do {
    switch (currentState) {
    case StateStarting:
        drawStartLogo();
        break;
      default:
        break;
    }
    drawStartLogo();
  } while (u8g.nextPage());

  delay(1000);
  secondsCount++;
}

