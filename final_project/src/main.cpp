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
  if (th_read() || toggle_sleep_mode() || drink_water()) {
    updateScreen();
  }
  /*
  
  
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
  }*/
}