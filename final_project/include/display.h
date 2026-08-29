#ifndef DISPLAY_H
#define DISPLAY_H

#include "pinout.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

void displayInit();
void displayString(String dispStr);
void displayLines(const String& line1,
                    const String& line2,
                    const String& line3,
                    const String& line4,
                    const String& line5);
void displayWakeup(const String &wakeupTime);
void displayDrinkWater();
void displayClear();
void I2CScan();

#endif