#ifndef SCREEN_H
#define SCREEN_H

#include <common.h>
#include <th.h>

//app state
enum ScreenState {
  SCREEN_DEFAULT,
  SCREEN_ALARM
};
ScreenState currentScreen = SCREEN_DEFAULT;

// tracking vars
int water_ct = 0;

// debounce vars
unsigned long lastLeftBtn = 0;
unsigned long lastMiddleBtn = 0;
unsigned long lastRightBtn = 0;
#define DEBOUNCE_DELAY 250

inline void updateScreen() {
  if (currentScreen == SCREEN_DEFAULT) {
    String l1 = "Temp: " + String(curr_temp, 1) + "C H:" + String(curr_hum, 1) + "%";
    String l2 = "Water: " + String(water_ct);
    String l3 = is_sleeping ? "Status: SLEEPING" : "Status: AWAKE";
    String l4 = is_sleeping ? "Moves: " + String(sleep_movement_ct) : "";
    displayLines(l1, l2, l3, l4);
  }
  else if (currentScreen == SCREEN_ALARM) {
    String l1 = "  ALARM  ";
    String l2 = "Wake: 07:00 AM";
    String l3 = "Drink: Every 2H";
    String l4 = "Right Btn -> Exit";
    displayLines(l1, l2, l3, l4);
  }
}

inline bool toggle_sleep_mode() {
  // left button: sleep mode toggle
  if (!digitalRead(BUTTON_LEFT) && (millis() - lastLeftBtn > DEBOUNCE_DELAY)) {
    is_sleeping = !is_sleeping;
    digitalWrite(LED_LEFT, is_sleeping);

    if (is_sleeping) {
      sleep_movement_ct = 0;
      displayString("Sleep Mode ON");

      /*if (deviceConnected) {
        pSleepChar->setValue(String(sleepMovementCount).c_str());
        pSleepChar->notify();
      }*/
      delay(1500);
    } else {
      displayString("Sleep Mode OFF");
      delay(1500);
    }

    lastLeftBtn = millis();
    return true;
  }

  return false;
}

#endif