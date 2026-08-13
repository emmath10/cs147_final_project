#ifndef PIR_H
#define PIR_H

#include <Arduino.h>
#include <pinout.h>

//timer for non-blocking code
unsigned long last_pir_trigger = 0;

//tracking var
int sleep_movement_ct = 0;
bool is_sleeping = false;

inline bool read_pir() {
  if (millis() - last_pir_trigger > 5000 && is_sleeping && digitalRead(PIR)) {
    sleep_movement_ct++;
    digitalWrite(LED_RIGHT, HIGH);
    delay(1000);
    digitalWrite(LED_RIGHT, LOW);
   
    /*if (deviceConnected) {
      pSleepChar->setValue(String(sleepMovementCount).c_str());
      pSleepChar->notify();
    }*/
    last_pir_trigger = millis();
    return true;
  }
  return false;
}

#endif