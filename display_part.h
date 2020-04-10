#ifndef display_part_h
#define display_part_h

#include <TFT_eSPI.h>
#include <SPI.h>
#include "WiFi.h"
#include <Wire.h>
#include "esp_adc_cal.h"

#ifndef TFT_DISPOFF
#define TFT_DISPOFF 0x28
#endif

#ifndef TFT_SLPIN
#define TFT_SLPIN   0x10
#endif

#define TFT_MOSI            19
#define TFT_SCLK            18
#define TFT_CS              5
#define TFT_DC              16
#define TFT_RST             23

#define TFT_BL          4  // Display backlight control pin
#define ADC_EN          14
#define ADC_PIN         34

TFT_eSPI tft = TFT_eSPI(135, 240); // Invoke custom library

void displaySetup()
{
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(2);
    //  tft.setTextColor(TFT_WHITE);
    tft.setCursor(0, 0);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(2);

    if (TFT_BL > 0) { // TFT_BL has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
        pinMode(TFT_BL, OUTPUT); // Set backlight pin to output mode
        digitalWrite(TFT_BL, TFT_BACKLIGHT_ON); // Turn backlight on. TFT_BACKLIGHT_ON has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
    }
}

void drawStringWithOffset(String string, int32_t dx, int32_t dy )
{
  tft.drawString(string, tft.width() / 2 + dx, tft.height() / 2 + dy);
}

void clearScreen()
{
  tft.fillScreen(TFT_BLACK);
}

void writeCenter(String str)
{
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString(str, tft.width() / 2, tft.height() / 2);
}

#endif