#ifndef BLE_H
#define BLE_H

#include "data.h"
#include "esp_bt.h" //used in main.cpp to optimize BLE
#include "pinout.h"
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <string>

// BLE UUIDs
#define SERVICE_UUID        "19b10000-e8f2-537e-4f6c-d104768a1214"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

void printValue(std::string value);
void initBLE();

#endif