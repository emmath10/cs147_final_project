#include "ble.h"

void printValue(std::string value) {
  // prints the value received from the BLE device to the Serial monitor
  Serial.println("*********");
  Serial.print("New value: ");
  for (int i = 0; i < value.length(); i++)
    Serial.print(value[i]);
  Serial.println();
  Serial.println("*********");
}

// BLE callbacks
class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) override {
    uint8_t *pData = pCharacteristic->getData();
    size_t len = pCharacteristic->getLength();

    //efficiently copies the value received into a buffer
    if (len > 0 && len < sizeof(curr_data.bleBuffer)) {
      memcpy(curr_data.bleBuffer, pData, len);
      curr_data.bleBuffer[len] = '\0';
      curr_data.bleNewData = true;
      tone(BUZZER, 2048, 1000);
    }
  }
};

// BLE initialization
void initBLE() {
  BLEDevice::init("Wellness Clock");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}