#ifndef TH_H
#define TH_H

#include <Arduino.h>
#include <Wire.h>
#include <DHT20.h>

// temp/hum sensor object
DHT20 dht20;

// tracking vars
float curr_hum = 0.0;
float curr_temp = 0.0;

inline bool th_init() {
  // initialize DHT20 sensor
  Wire.begin();
  if (!dht20.begin()) {
    Serial.println("Failed to find DHT20 sensor");
    return false;
  }
  return true;
}

inline bool th_read() {
  if (millis() - dht20.lastRead() > 2000) {
    dht20.read();
    float hum = dht20.getHumidity();
    float temp = dht20.getTemperature();

    if (curr_hum != hum || curr_temp != temp) {
      curr_hum = hum;
      curr_temp = temp;

      /*if (deviceConnected) {
        pTempChar->setValue(String(currentTemp, 1).c_str());
        pTempChar->notify();
        pHumChar->setValue(String(currentHum, 1).c_str());
        pHumChar->notify();
      }*/
    }
    return true;
  }
  return false;
}

void th_print() {
  Serial.print("Temperature: ");
  Serial.print(curr_temp, 1);
  Serial.println("°");
  Serial.print("Humidity: ");
  Serial.print(curr_hum, 1);
  Serial.println("%");
  Serial.println("---------------------");
}

#endif