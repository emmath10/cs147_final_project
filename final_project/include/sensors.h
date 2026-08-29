#ifndef SENSORS_H
#define SENSORS_H

#include "data.h"
#include "pinout.h"
#include <Arduino.h>
#include <DHT20.h>
#include <Wire.h>


//---------PIR Sensor---------//

bool pirRead(Data *data);

//---------Temperature/Humidity Sensor---------//


bool thInit();
bool thRead(Data *data);
inline void thPrint(Data *data);

#endif