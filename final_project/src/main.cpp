#include <Arduino.h>
#include <Wire.h>
#include <pinout.h>
#include <displayControl.h>
#include <DHT20.h>


//BLE libraries
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

//BLE callbacks
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

//initialize the temp/hum sensor
Adafruit_AHTX0 dht20;

//app state
enum ScreenState {
  SCREEN_DEFAULT,
  SCREEN_ALARM
};
ScreenState currentScreen = SCREEN_DEFAULT;
bool isSleeping = false;

//tracking vars
int waterCount = 0;
int sleepMovementCount = 0;
float currentTemp = 0.0;
float currentHum = 0.0;

//timer for non-blocking code
unsigned long lastSensorRead = 0;
unsigned long lastPirTrigger = 0;

//debounce vars
unsigned long lastLeftBtn = 0;
unsigned long lastMiddleBtn = 0;  
unsigned long lastRightBtn = 0;
const int debounceDelay = 250;

//func declarations
void updateScreen();
void initBLE();

void setup() {
  Serial.begin(115200);

  //initialize GPIO pins
  pinMode(PIR, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_MIDDLE, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(LED_LEFT, OUTPUT);
  pinMode(LED_MIDDLE, OUTPUT);
  pinMode(LED_RIGHT, OUTPUT);

  //initialize OLED
  displayInit();
  displayString("Wellness\nClock\nStarting...");

  //initialize DHT20 sensor
  if (!dht20.begin()) {
    Serial.println("Failed to find AHT20 sensor");
  }

  //initialize BLE
  initBLE();
  delay(1000);
  updateScreen();
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastSensorRead > 2000) {
    sensors_event_t humidity, temp;
    dht20.getEvent(&humidity, &temp);

    if (currentTemp != temp.temperature || currentHum != humidity.relative_humidity) {
      currentTemp = temp.temperature;
      currentHum = humidity.relative_humidity;
      updateScreen();

      if (deviceConnected) {
        pTempChar->setValue(String(currentTemp, 1).c_str());
        pTempChar->notify();
        pHumChar->setValue(String(currentHum, 1).c_str());
        pHumChar->notify();
      }
    }
      lastSensorRead = currentMillis;
  }
  if (digitalRead(PIR) == HIGH) {
    if (currentMillis - lastPirTrigger > 3000) {
      if (isSleeping) {
        sleepMovementCount++;
        digitalWrite(LED_RIGHT, HIGH);
        delay(100);
        digitalWrite(LED_RIGHT, LOW);
        updateScreen();
        
        if (deviceConnected) {
          pSleepChar->setValue(String(sleepMovementCount).c_str());
          pSleepChar->notify();
        }
      }
      lastPirTrigger = currentMillis;
    }
  }


  //left button: sleep mode toggle
  if (digitalRead(BUTTON_LEFT) == LOW && (currentMillis - lastLeftBtn > debounceDelay)) {
    isSleeping = !isSleeping;
    digitalWrite(LED_LEFT, isSleeping ? HIGH : LOW);
    
    if (isSleeping) {
      sleepMovementCount = 0;
      displayString("Sleep Mode");
    
      if (deviceConnected) {
        pSleepChar->setValue(String(sleepMovementCount).c_str());
        pSleepChar->notify();
      }
      delay(1500);
    } else {
        displayString("Wellness Clock");
        delay(1500);
    }

    lastLeftBtn = currentMillis;
    updateScreen();
  }
  
  //middle button: water drinking count
  if (digitalRead(BUTTON_MIDDLE) == LOW && (currentMillis - lastMiddleBtn > debounceDelay)) {
    waterCount++;
    digitalWrite(LED_MIDDLE, HIGH);
    tone(BUZZER, 1000, 150);
    delay(150);
    digitalWrite(LED_MIDDLE, LOW);
    updateScreen();
    
    if (deviceConnected) {
      pWaterChar->setValue(String(waterCount).c_str());
      pWaterChar->notify();
    }
    lastMiddleBtn = currentMillis;
  }
  
  //right button: toggle UI screen
  if (digitalRead(BUTTON_RIGHT) == LOW && (currentMillis - lastRightBtn > debounceDelay)) {
    if (currentScreen == SCREEN_DEFAULT) {
      currentScreen = SCREEN_ALARM;
    } else {
      currentScreen = SCREEN_DEFAULT;
    }

    lastRightBtn = currentMillis;
    updateScreen();
  }
}


void updateScreen() {
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