#include "ble.h"
#include "cloud.h"
#include "data.h"
#include "iotWifi.h"
#include "pinout.h"
#include "realTime.h"
#include "screen.h"
#include "sensors.h"

long wakeupCooldown, waterCooldown;

Data data;

void setup() {
  Serial.begin(115200);

  //initialize GPIO pins
  pinMode(PIR, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BTN_R, INPUT_PULLUP);
  pinMode(BTN_M, INPUT_PULLUP);
  pinMode(BTN_L, INPUT_PULLUP);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_M, OUTPUT);
  pinMode(LED_L, OUTPUT);

  //actuator feedback indicating duration of setup
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_M, HIGH);
  digitalWrite(LED_L, HIGH);
  tone(BUZZER, 2048);

  //initialize T/H sensor
  thInit();
  
  //initialize OLED
  displayInit();
  displayString("Wellness\nClock\nStarting...");

  //initialize BLE
  esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
  initBLE();

  //initialize WiFi
  wifiInit();

  //initialize and get the time from NTP
  initTime(&data);

  delay(1000);

  //start displaying screen
  updateScreen(&data);

  //turn off actuators to signal end of setup
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_M, LOW);       
  digitalWrite(LED_L, LOW);
  noTone(BUZZER);
}

void loop() {
  //alarms
  triggerWakeupAlarm(&data, &wakeupCooldown);
  triggerWaterAlarm(&data, &waterCooldown);

  //sensors
  pirRead(&data);
  thRead(&data);

  //button presses
  toggleSleepMode(&data);
  toggleUIScreen();
  drinkWater(&data);

  updateScreen(&data);

  //communication protocols
  sendTelemetry(&data);

  //parse command from phone if ESP32 received valid data
  if (data.bleNewData) {
    data.bleNewData = false;
    printValue(std::string(data.bleBuffer));
    toggleCMD(std::string(data.bleBuffer), &data);
  }
}