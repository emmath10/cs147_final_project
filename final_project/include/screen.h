#ifndef SCREEN_H
#define SCREEN_H

#include "data.h"
#include "display.h"
#include "pinout.h"
#include "realTime.h"
#include "sensors.h"
#include <algorithm>
#include <cctype>
#include <string>

#define DEBOUNCE_DELAY 250

#define TIME_BEFORE_SNOOZE 1000 * 60
#define RING_INTERVAL 2000

//app state
enum ScreenState {
  SCREEN_DEFAULT,
  SCREEN_ALARM
};

void updateScreen(Data *data);
bool drinkWater(Data *data);
bool toggleUIScreen();
bool toggleSleepMode(Data *data);
void toggleCMD(std::string value, Data *data);
bool ringAlarm(Data *data);
void triggerWakeupAlarm(Data *data, long *wakeupCooldown);
void triggerWaterAlarm(Data *data, long *waterCooldown);

#endif