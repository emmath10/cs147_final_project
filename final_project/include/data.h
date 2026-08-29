#ifndef DATA_H
#define DATA_H

#include <Arduino.h>

struct Data {
  //temperature/humidity sensor data
  float curr_hum = 0.0;
  float curr_temp = 0.0;

  //sleep movement data
  int sleep_movement_ct = 0;
  bool is_sleeping = false;

  //hydration data
  int water_ct = 0;

  //alarms
  tm wakeupAlarm; //stores the time when the wakeup alarm triggers
  tm lastWater;   //stores when the last time the user drank water
  int waterInterval = 0;  //the number of hours that pass after the user last drank water before the water alarm triggers

  //BLE cmd
  char bleBuffer[64]; //stores the new BLE cmd received
  volatile bool bleNewData = false;

  //resets water_ct to zero at midnight
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

//defined in main.cpp
//to be used in other headers
extern Data data;

#endif