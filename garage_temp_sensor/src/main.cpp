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

int lastTemp = 0;
void displayTemperature(float main_temp) {
  tft.setTextSize(3);

  tft.setTextColor(BLACK);
  tft.setCursor(5, 20);
  String temperatureValue = String(lastTemp) + (char)247 + "C";
  tft.print(temperatureValue);

  tft.setTextColor(WHITE);
  tft.setCursor(5, 20);
  temperatureValue = String(int(main_temp)) + (char)247 + "C";
  tft.print(temperatureValue);

  lastTemp = int(main_temp);
}

int lastHum = 0;
void displayHumidity(float h) {
  tft.setTextSize(3);

  tft.setTextColor(BLACK);
  tft.setCursor(5, 100);
  String value = String(lastHum) + "%";
  tft.print(value);

  tft.setTextColor(WHITE);
  tft.setCursor(5, 100);
  
  lastHum = int(h);
  value = String(lastHum) + "%";
  tft.print(value);
}