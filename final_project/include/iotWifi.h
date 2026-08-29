#ifndef IOT_WIFI_H
#define IOT_WIFI_H

#include <WiFi.h>

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

// https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html

inline void wifiInit() {
  WiFi.mode(WIFI_STA);
  delay(1000);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    Serial.print(WiFi.status());
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("MAC address: ");
  Serial.println(WiFi.macAddress());
}

#endif