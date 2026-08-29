#ifndef CLOUD_H
#define CLOUD_H

#include "data.h"
#include "screen.h"
#include "sensors.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <ctime>

// Telemetry interval
#define TELEMETRY_INTERVAL 3000 // Send data every 3 seconds

void createPayload(char *buffer, size_t bufferSize, Data *data);
void sendTelemetry(Data *data);

#endif