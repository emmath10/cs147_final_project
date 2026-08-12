#include <../include/ble.h>

//BLE callbacks
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

//BLE initialization
void initBLE() {
  BLEDevice::init("Wellness Clock");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  pTempChar = pService->createCharacteristic(TEMP_CHAR_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  pHumChar = pService->createCharacteristic(HUM_CHAR_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  pWaterChar = pService->createCharacteristic(WATER_CHAR_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  pSleepChar = pService->createCharacteristic(SLEEP_CHAR_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

  pTempChar->addDescriptor(new BLE2902());
  pHumChar->addDescriptor(new BLE2902());
  pWaterChar->addDescriptor(new BLE2902());
  pSleepChar->addDescriptor(new BLE2902());

  //broadcast initial data
  pTempChar->setValue("0,0");
  pHumChar->setValue("0,0");
  pWaterChar->setValue(String(waterCount).c_str());
  pSleepChar->setValue(String(sleepMovementCount).c_str());

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
}