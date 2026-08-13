/*#ifndef SCREEN_H
#define SCREEN_H

//app state
enum ScreenState {
  SCREEN_DEFAULT,
  SCREEN_ALARM
};
ScreenState currentScreen = SCREEN_DEFAULT;

inline void updateScreen() {
    if (currentScreen == SCREEN_DEFAULT) {
        String l1 = "Temp: " + String(currentTemp, 1) + "C H:" + String(currentHum, 1) + "%";
        String l2 = "Water: " + String(waterCount);
        String l3 = isSleeping ? "Status: SLEEPING" : "Status: AWAKE";
        String l4 = isSleeping ? "Moves: " + String(sleepMovementCount) : "";
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

#endif*/