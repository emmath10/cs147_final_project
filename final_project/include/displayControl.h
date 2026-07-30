#ifndef DISPLAY_CONTROL_H
#define DISPLAY_CONTROL_H

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <pinout.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

void displayInit();
void displayString(String dispStr);
void displayLines(const String& line1,
                    const String& line2,
                    const String& line3,
                    const String& line4);
void displayClear();
void I2CScan();

#endif