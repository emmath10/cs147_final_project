#include <common.h>

//tracking vars
int waterCount = 0;

//debounce vars
unsigned long lastLeftBtn = 0;
unsigned long lastMiddleBtn = 0;  
unsigned long lastRightBtn = 0;
const int debounceDelay = 250;

void setup() {
  Serial.begin(115200);
  th_init();

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
  
  //initialize BLE
  //initBLE();
  delay(1000);
  //updateScreen();
  for (int i = 60; i > -1; i--) {
    displayString(String(i));
    delay(1000);
  }
}

void loop() {
  if (read_pir()) {
    displayString(String(sleep_movement_ct));
  }

  /*//left button: sleep mode toggle
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
  }*/
}