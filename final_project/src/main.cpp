#include <common.h>

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

//timer for non-blocking code
unsigned long lastPirTrigger = 0;

//debounce vars
unsigned long lastLeftBtn = 0;
unsigned long lastMiddleBtn = 0;  
unsigned long lastRightBtn = 0;
const int debounceDelay = 250;

//func declarations
void updateScreen();

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

  //initialize BLE
  initBLE();
  delay(1000);
  updateScreen();
}

void loop() {
  unsigned long currentMillis = millis();
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