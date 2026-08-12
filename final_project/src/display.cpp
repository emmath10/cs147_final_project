#include <display.h>

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void displayInit() {
    Wire.begin(21, 22);
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }
}

void displayString(String dispStr) {
    displayClear();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(dispStr);
    display.display();
    //display.setTextSize(1); if needed to reset back to normal size
}

void displayLines(const String& line1,
                    const String& line2 = "",
                    const String& line3 = "",
                    const String& line4 = "") {
    displayClear();
    display.setCursor(0, 0);
    display.println(line1);
    if (line2.length()) display.println(line2);
    if (line3.length()) display.println(line3);
    if (line4.length()) display.println(line4);
    display.display();
}

void displayClear() {
    display.clearDisplay();
}

void I2CScan() {
    Serial.println("Scanning I2C bus...");
    int count = 0;
    for (uint8_t addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {
        Serial.print("Found I2C device at 0x");
        if (addr < 16) Serial.print("0");
        Serial.println(addr, HEX);
        count++;
        }
    }
    Serial.print("I2C devices found: ");
    Serial.println(count);
}