#ifndef DATA_H
#define DATA_H

#include <Arduino.h>

struct Data {
  int sleep_movement_ct = 0;
  bool is_sleeping = false;
  float curr_hum = 50.0;
  float curr_temp = 50.0;
  int water_ct = 0;

  tm wakeupAlarm;
  tm lastWater;
  int waterInterval = 0;

  char bleBuffer[64];
  volatile bool bleNewData = false;

  inline void resetWaterCT() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }
    if (timeinfo.tm_hour == 0 && timeinfo.tm_min == 0 &&
        timeinfo.tm_sec == 0) {
          water_ct = 0;
        }
  }
};

extern Data data;

#endif