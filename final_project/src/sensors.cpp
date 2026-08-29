#include "sensors.h"

// timer for non-blocking code
unsigned long lastPIRTrigger = 0;

// temp/hum sensor object
DHT20 dht20;


//---------PIR Sensor Functions---------//


bool pir_read(Data * data) {
  if (data->isSleeping && (millis() - lastPIRTrigger > 5000) &&
      digitalRead(PIR)) {
    data->sleepMovementCt++;
    digitalWrite(LED_R, HIGH);
    delay(1000);
    digitalWrite(LED_R, LOW);
    lastPIRTrigger = millis();
    return true;
  }
  return false;
}


//---------Temperature/Humidity Sensor Functions---------//


bool thInit() {
  // initialize DHT20 sensor
  Wire.begin();
  if (!dht20.begin()) {
    Serial.println("Failed to find DHT20 sensor");
    return false;
  }
  return true;
}

bool thRead(Data * data) {
  if (millis() - dht20.lastRead() > 2000) {
    dht20.read();
    float hum = dht20.getHumidity();
    float temp = dht20.getTemperature();

    if (data->currHum != hum || data->currTemp != temp) {
      data->currHum = hum;
      data->currTemp = temp;
    }

    return true;
  }
  return false;
}

inline void th_print(Data * data) {
  Serial.print("Temperature: ");
  Serial.print(data->currTemp, 1);
  Serial.println("°");
  Serial.print("Humidity: ");
  Serial.print(data->currHum, 1);
  Serial.println("%");
  Serial.println("---------------------");
}