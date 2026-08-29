#include "ble.h"
#include "cloud.h"
#include "data.h"
#include "pinout.h"
#include "realTime.h"
#include "screen.h"
#include "sensors.h"

long wakeupCooldown, waterCooldown;

Data curr_data;

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

  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_M, HIGH);
  digitalWrite(LED_L, HIGH);
  tone(BUZZER, 2048, 1000);

  //initialize T/H sensor
  th_init();
  
  //initialize OLED
  displayInit();
  displayString("Wellness\nClock\nStarting...");

  //initialize BLE
  esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
  initBLE();

  //initialize WiFi
  wifiInit();

  // Initialize and get the time from NTP
  configTime(GMT_OFFSET_SEC, 0, NTP_SERVER);
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
  }
  curr_data.lastWater.tm_hour = timeinfo.tm_hour;
  curr_data.lastWater.tm_min = timeinfo.tm_min;
  curr_data.waterInterval = 2;

  delay(1000);
  updateScreen(&curr_data);

  digitalWrite(LED_R, LOW);
  digitalWrite(LED_M, LOW);       
  digitalWrite(LED_L, LOW);
}

void loop() {
  triggerWakeupAlarm(&curr_data, &wakeupCooldown);
  triggerWaterAlarm(&curr_data, &waterCooldown);
  pir_read(&curr_data);
  th_read(&curr_data);
  toggle_sleep_mode(&curr_data);
  toggle_UI_screen();
  drink_water(&curr_data);
  updateScreen(&curr_data);
  sendTelemetry(&curr_data);
  if (curr_data.bleNewData) {
    curr_data.bleNewData = false;
    printValue(std::string(curr_data.bleBuffer));
    toggleCMD(std::string(curr_data.bleBuffer), &curr_data);
  }
}