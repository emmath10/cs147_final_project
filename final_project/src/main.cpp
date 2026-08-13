#include <common.h>

void setup() {
  Serial.begin(115200);
  th_init();

  //initialize GPIO pins
  pinMode(PIR, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BTN_R, INPUT_PULLUP);
  pinMode(BTN_M, INPUT_PULLUP);
  pinMode(BTN_L, INPUT_PULLUP);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_M, OUTPUT);
  pinMode(LED_L, OUTPUT);

  //initialize OLED
  displayInit();
  displayString("Wellness\nClock\nStarting...");

  //initialize BLE
  //initBLE();
  delay(1000);
  updateScreen();
}

void loop() {
  if (th_read() || toggle_sleep_mode() ||
      toggle_UI_screen() || drink_water()) {
    updateScreen();
  }
}