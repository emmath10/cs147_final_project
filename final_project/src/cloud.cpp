#include "cloud.h"

//"SharedAccessSignature sr=.........”
#define SAS_TOKEN ""

// generate certificate: openssl s_client -showcerts -connect cs147g3IotHub.azure-devices.net:443
const char *root_ca = "";
const String iothubName = "cs147g3IotHub";
const String deviceName = "cs147g3esp32";
const String url = "https://" + iothubName + ".azure-devices.net/devices/" +
                   deviceName + "/messages/events?api-version=2021-04-12";

// monitor command: az iot hub monitor-events --hub-name cs147g3IotHub --device-id cs147g3esp32 --properties all

uint32_t lastTelemetryTime = 0;

void createPayload(char *buffer, size_t bufferSize, Data * data) {
  ArduinoJson::JsonDocument doc;
  //doc["time"] = long(time(NULL));
  doc["tempC"] = data->curr_temp;
  doc["hum"] = data->curr_hum;
  doc["isSleeping"] = data->is_sleeping;
  doc["sleepMovementCt"] = data->sleep_movement_ct;
  doc["water_ct"] = data->water_ct;
  serializeJson(doc, buffer, bufferSize);
}

void sendTelemetry(Data *data) {
  if (millis() - lastTelemetryTime < TELEMETRY_INTERVAL) {
    return;
  }
  // Send telemetry via HTTPS
  char buffer[256];
  createPayload(buffer, 256, data);

  WiFiClientSecure client;
  client.setCACert(root_ca); // Set root CA certificate

  HTTPClient http;
  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", SAS_TOKEN);
  int httpCode = http.POST(buffer);

  if (httpCode ==
      204) { // IoT Hub returns 204 No Content for successful telemetry
    Serial.println("Telemetry sent: " + String(buffer));
  } else {
    Serial.println("Failed to send telemetry. HTTP code: " + String(httpCode));
  }
  http.end();

  lastTelemetryTime = millis();
}