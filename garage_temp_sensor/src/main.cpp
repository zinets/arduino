#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include "Adafruit_I2CDevice.h"

#include "DHT.h"

#define DHTPIN D2 
#define DHTTYPE DHT11 

#define cs   D8
#define dc   D3
#define rst  D4

uint32_t delayMS;

Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);
DHT dht(DHTPIN, DHTTYPE);

void displayTemperature(float main_temp);
void displayHumidity(float h);

#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0
#define WHITE    0xFFFF
#define GREY     0xC618

void setup(void) {
  Serial.begin(9600);

  Serial.println(F("DHTxx test!"));
  dht.begin();

  Wire.begin();

  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  tft.fillScreen(BLACK);
  tft.setCursor(30, 80);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.fillScreen(BLACK);
  tft.setCursor(30, 80);
  tft.setTextColor(WHITE);
  
  tft.drawLine(0, 80, 127, 80, GREY);
}

void loop() {
  delay(5000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t))  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));

  displayTemperature(t);
  displayHumidity(h);
}

float lastTemp = 0;
void displayTemperature(float main_temp) {
  if (main_temp == lastTemp) {
    return;
  }

  int scale = 5;
  tft.setTextSize(scale);

  int x = 5;
  int y = 80 - 2 - scale * 8;
  tft.setTextColor(BLACK);
  tft.setCursor(x, y);
  String temperatureValue = String(lastTemp, 0); 
  tft.print(temperatureValue);

  lastTemp = int(main_temp);
  tft.setTextColor(WHITE);
  tft.setCursor(x, y);
  temperatureValue = String(lastTemp, 0); 
  tft.print(temperatureValue);

  int textScale = scale - 2;
  tft.setTextSize(textScale);
  x += 2 * 6 * scale + 4;
  y = 80 - 2 - textScale * 8;
  tft.setCursor(x, y);
  String sign = String((char)247) + "C";  
  tft.print(sign);
}

float lastHum = 0;
void displayHumidity(float h) {
  if (lastHum == h) {
    return;
  }

  int scale = 5;
  tft.setTextSize(scale);

  int x = 5;
  int y = 80 + 6;
  tft.setTextColor(BLACK);
  tft.setCursor(x, y);
  String value = String(lastHum, 0); 
  tft.print(value);

  lastHum = int(h);
  tft.setTextColor(WHITE);
  tft.setCursor(x, y);
  value = String(lastHum, 0); 
  tft.print(value);

  int textScale = scale - 2;
  tft.setTextSize(textScale);
  x += 2 * 6 * scale + 4;
  y = 80 + 6;
  tft.setCursor(x, y);
  String sign = "%";  
  tft.print(sign);
}