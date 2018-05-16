/*
  Adafruit_GFX_Minimum_Sketch
  
  Code to test the screen or a starting point for working on screen operations using the Adafruit GFX library primatives.
  
  Trent Dye 2018
  
 */

#include <Adafruit_GFX.h> 
#include <Adafruit_ST7735.h>
#include <SPI.h>

#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF

#define TFT_CS     10
#define TFT_RST    9  
#define TFT_DC     8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

void setup() {
  
  // put your setup code here, to run once:
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  tft.fillScreen(RED);
  delay(500);
  tft.fillScreen(WHITE);
  delay(500);
}
