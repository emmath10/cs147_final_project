#ifndef BLE_H
#define BLE_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

//BLE UUIDs
#define SERVICE_UUID    "19b10000-e8f2-537e-4f6c-d104768a1214"
#define TEMP_CHAR_UUID  "19b10000-e8f2-537e-4f6c-d104768a1214"
#define HUM_CHAR_UUID   "19b10000-e8f2-537e-4f6c-d104768a1214"
#define WATER_CHAR_UUID "19b10000-e8f2-537e-4f6c-d104768a1214"
#define SLEEP_CHAR_UUID "19b10000-e8f2-537e-4f6c-d104768a1214"

BLEServer* pServer = NULL;
BLECharacteristic* pTempChar = NULL;
BLECharacteristic* pHumChar = NULL;
BLECharacteristic* pWaterChar = NULL;
BLECharacteristic* pSleepChar = NULL;
bool deviceConnected = false;

void initBLE();

#endif