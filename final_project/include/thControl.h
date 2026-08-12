#ifndef TH_H
#define TH_H

#include <Arduino.h>
#include <Wire.h>
#include <DHT20.h>

//tracking vars
float currentTemp = 0.0;
float currentHum = 0.0;

//timer for non-blocking code
unsigned long lastSensorRead = 0;

void thInit();
void readTH(unsigned long currentMillis);
void printTHData(uint8_t temp, uint8_t humidity);

#endif