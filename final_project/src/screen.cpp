#include "screen.h"
#include "realTime.h"

// debounce vars
unsigned long last_btn_r = 0;
unsigned long last_btn_m = 0;
unsigned long last_btn_l = 0;

ScreenState curr_screen = SCREEN_DEFAULT;

void updateScreen(Data * data) {
  if (curr_screen == SCREEN_DEFAULT) {
    String l1 = getTime() + "\n";
    String l2 = "Temp:" + String(data->curr_temp, 1) + "C  " +
                "Hum:" + String(data->curr_hum, 1) + "%\n";
    String l3 = "Water Count: " + String(data->water_ct);
    String l4 = data->is_sleeping ? "Status: SLEEPING" : "Status: AWAKE";
    String l5 = data->is_sleeping ? "Sleep Moves: " + String(data->sleep_movement_ct) : "";
    displayLines(l1, l2, l3, l4, l5);
  } else if (curr_screen == SCREEN_ALARM) {
    String l1 = "       ALARM";
    String l2 = "Wake Up: " + displayAlarm(&(data->wakeupAlarm));
    String l3 = "Hydrate: Every " + String(data->waterInterval) + " Hr";
    String l4 = "\nHelp: tinyurl.com/\nWellnessClock";
    String l5 = "\n Middle Btn -> Exit ";
    displayLines(l1, l2, l3, l4, l5);
  }
}

bool drink_water(Data *data) {
  // right button: water drinking count
  if (!digitalRead(BTN_R) && (millis() - last_btn_r > DEBOUNCE_DELAY)) {
    while (!digitalRead(BTN_R)) {
    }
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return false;
    }
    data->water_ct++;
    data->lastWater.tm_hour = timeinfo.tm_hour;
    digitalWrite(LED_R, HIGH);
    tone(BUZZER, 2048, 500);
    delay(500);
    digitalWrite(LED_R, LOW);
    updateScreen(data);

    last_btn_r = millis();
    return true;
  }
  return false;
}

bool toggle_UI_screen() {
  // middle button: toggle UI screen
  if (digitalRead(BTN_M) == LOW && (millis() - last_btn_r > DEBOUNCE_DELAY)) {
    tone(BUZZER, 2048, 1000);
    while (!digitalRead(BTN_M)) {
    }
    digitalWrite(LED_M, HIGH);
    if (curr_screen == SCREEN_DEFAULT) {
      curr_screen = SCREEN_ALARM;
    } else {
      curr_screen = SCREEN_DEFAULT;
    }
    delay(1000);
    digitalWrite(LED_M, LOW);

    last_btn_m = millis();
    return true;
  }
  return false;
}

bool toggle_sleep_mode(Data * data) {
  // left button: sleep mode toggle
  if (!digitalRead(BTN_L) && (millis() - last_btn_l > DEBOUNCE_DELAY)) {
    tone(BUZZER, 2048, 1000);
    while (!digitalRead(BTN_L)) {
    }
    data->is_sleeping = !data->is_sleeping;
    digitalWrite(LED_L, data->is_sleeping);

    if (data->is_sleeping) {
      data->sleep_movement_ct = 0;
      displayString("Sleep Mode ON");
      delay(1500);
    } else {
      displayString("Sleep Mode OFF");
      delay(1500);
    }

    last_btn_l = millis();
    return true;
  }

  return false;
}

// https://cplusplus.com/reference/string/string/find/

void parseCMD(std::string value, std::string * buf) {
  transform(value.begin(), value.end(), value.begin(), ::tolower);
  size_t alarm_cmd = value.find("alarm");
  size_t water_cmd = value.find("water");
  if (alarm_cmd != std::string::npos) {
    buf[0] = "alarm";
    buf[1] = value.substr(6, 2);
    buf[2] = value.substr(9, 2);
  } else if (water_cmd != std::string::npos) {
    buf[0] = "water";
    buf[1] = value[6];
  }
}

void toggleCMD(std::string value, Data *data) {
  /* commands:
  - set alarm time: alarm [HH]:[MM]
      example: alarm 07:00
  - set water alarm time intervals: water H
      example: water 2
        -> clock reminds you to drink water every 2 hours
  */
  std::string buf[3];
  parseCMD(value, buf);
  if (buf[0] == "alarm") {
    data->wakeupAlarm.tm_hour = std::stoi(buf[1]);
    data->wakeupAlarm.tm_min = std::stoi(buf[2]);
    digitalWrite(LED_L, HIGH);
  } else if (buf[0] == "water") {
    data->waterInterval = std::stoi(buf[1]);
    digitalWrite(LED_R, HIGH);
  }
  updateScreen(data);
  delay(1000);
  digitalWrite(LED_L, LOW);
  digitalWrite(LED_L, LOW);
}

bool ringAlarm(Data *data) {
  float snooze = millis() + TIME_BEFORE_SNOOZE;
  float ring = millis();
  bool onOff = false;
  while (millis() < snooze) {
    if (millis() > ring) {
      onOff = !onOff;
      digitalWrite(LED_R, onOff);
      digitalWrite(LED_M, onOff);
      digitalWrite(LED_L, onOff);
      if (onOff) {
        tone(BUZZER, 2048);
      } else {
        noTone(BUZZER);
      }
      ring += 2000;
    }
    if (!digitalRead(BTN_R) || !digitalRead(BTN_M) || !digitalRead(BTN_L)) {
      digitalWrite(LED_R, LOW);
      digitalWrite(LED_M, LOW);
      digitalWrite(LED_L, LOW);
      noTone(BUZZER);
      updateScreen(data);
      return true;
    }
  }
  return false;
}

void triggerWakeupAlarm(Data *data, long * wakeupCooldown) {
  if (millis() <= *wakeupCooldown) { return; }
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  if (timeinfo.tm_hour == data->wakeupAlarm.tm_hour &&
      timeinfo.tm_min == data->wakeupAlarm.tm_min) {
    displayWakeup(getTime());
    ringAlarm(data);
    *wakeupCooldown = millis() + 60000;
  }
}

void triggerWaterAlarm(Data *data, long * waterCooldown) {
  if (millis() <= *waterCooldown) { return; }
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  if (data->lastWater.tm_hour + data->waterInterval >= timeinfo.tm_hour) {
    displayDrinkWater();
    if (ringAlarm(data)) {
      (data->water_ct)++;
    }
    data->lastWater.tm_hour = timeinfo.tm_hour;
    *waterCooldown = millis() + 60000;
  }
}