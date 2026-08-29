#ifndef REAL_TIME_H
#define REAL_TIME_H

#include "iotWifi.h"
#include <ctime>
#include <Arduino.h>

#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SEC -7 * 60 * 60
#define PREPEND_ZERO(n) n < 10 ? "0" + String(n) : String(n)

// en.cppreference.com/c/chrono/time
inline void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  // Print formatted time (e.g., 2026-08-26 15:30:00)
  Serial.println(&timeinfo, "%Y-%m-%d %H:%M:%S");
}

inline String getTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return "";
  }

  String hr = String(PREPEND_ZERO(timeinfo.tm_hour));
  String min = String(PREPEND_ZERO(timeinfo.tm_min));
  String sec = String(PREPEND_ZERO(timeinfo.tm_sec));
  String m = timeinfo.tm_hour > 12 ? "PM" : "AM";

  return hr + ":" + min + ":" + sec + " " + m;
}

inline String displayAlarm(tm * alarm) {
  String hr = String(PREPEND_ZERO(alarm->tm_hour));
  String min = String(PREPEND_ZERO(alarm->tm_min));
  String m = alarm->tm_hour > 12 ? "PM" : "AM";
  return hr + ":" + min + " " + m;
}

#endif