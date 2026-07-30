#include <Arduino.h>
#include <Wire.h>
#include <pinout.h>

void thInit();
void writeToTH(uint8_t msg);
uint8_t readByteFromTH();
void readFromTH(uint8_t * data);
uint8_t calcHumidity(uint8_t * data);
uint8_t calcTemp(uint8_t * data);
void printTHData(uint8_t temp, uint8_t humidity);