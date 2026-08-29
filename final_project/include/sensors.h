#ifndef SENSORS_H
#define SENSORS_H

#include "data.h"
#include "pinout.h"
#include <Arduino.h>
#include <DHT20.h>
#include <Wire.h>


//---------PIR Sensor---------//

bool pir_read(Data *data);

//---------Temperature/Humidity Sensor---------//


bool th_init();
bool th_read(Data *data);
inline void th_print(Data *data);

#endif