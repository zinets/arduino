#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#include <Fonts/FreeMonoBold24pt7b.h>

#define TFT_CS      15  // D8
#define TFT_RST     4   // D2
#define TFT_DC      5   // A0 -> D1
// #define TFT_SDA         D7
// #define TFT_SCK         D5
// LED - 3.3 !!! 

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

const int inset = 3;

void drawUkr() {
  tft.setFont(&FreeMonoBold24pt7b);
  tft.setTextSize(1);

  tft.fillRect(0, 0, 160, 64, ST7735_BLUE);
  tft.fillRect(0, 64, 160, 64, ST7735_YELLOW);

  int w = 28;
  int x = (160 - 2 * w) / 2;
  tft.fillRect(x - inset, 52 - inset, 2 * w + 2 * inset, 24 + 2 * inset, ST7735_BLACK);

  tft.setCursor(x, 52 + 24);
  tft.print("UA");
}

void drawEng() {
  tft.setFont(&FreeMonoBold24pt7b);
  tft.setTextSize(1);

  tft.fillRect(0, 0, 160, 64, ST7735_BLACK);
  tft.fillRect(0, 64, 160, 64, ST7735_BLACK);

  int w = 28;
  int x = (160 - 2 * w) / 2;
  tft.fillRect(x - inset, 52 - inset, 2 * w + 2 * inset, 24 + 2 * inset, ST7735_BLACK);

  tft.setCursor(x, 52 + 24);
  tft.print("EN");
}

void drawRus() {
  tft.setFont(&FreeMonoBold24pt7b);
  tft.setTextSize(1);

  tft.fillRect(0, 0, 160, 43, ST7735_WHITE);
  tft.fillRect(0, 43, 160, 43, ST7735_BLUE);
  tft.fillRect(0, 86, 160, 44, ST7735_RED);

  int w = 28;
  int x = (160 - 2 * w) / 2;
  tft.fillRect(x - inset, 52 - inset, 2 * w + 2 * inset, 24 + 2 * inset, ST7735_BLACK);

  tft.setCursor(x, 52 + 24);
  tft.print("RU");

  tft.setFont();
  tft.setTextSize(2);
  tft.setCursor(50, 13);
  tft.setTextColor(ST7735_BLACK);
  tft.print("putin");
  tft.setCursor(62, 99);
  tft.setTextColor(ST7735_WHITE);
  tft.print("huy");
}


void setup() {  
  Serial.begin(9600);
  
  tft.initR(INITR_BLACKTAB);     
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(1);
  

  drawEng();
}



int lastLayout = 0;

void loop() {
  if (Serial.available() > 0) {
    int incoming = Serial.read();
    if (incoming != lastLayout) {
      lastLayout = incoming;
      switch (incoming) {
        case 1: 
          drawEng();
          break;
        case 2: 
          drawRus();
          break;
        case 3:
          drawUkr();
          break;
        default:
          // tft.fillScreen(ST7735_BLACK);
          break;
      }
    }

    // tft.setFont();
    // tft.setCursor(0, 0);
    // tft.setTextSize(1);
    // tft.print(incoming);
  }
}