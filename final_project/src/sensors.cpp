#include "sensors.h"

// timer for non-blocking code
unsigned long last_pir_trigger = 0;

// temp/hum sensor object
DHT20 dht20;


//---------PIR Sensor Functions---------//


bool pir_read(Data * data) {
  if (data->is_sleeping && (millis() - last_pir_trigger > 5000) &&
      digitalRead(PIR)) {
    data->sleep_movement_ct++;
    digitalWrite(LED_R, HIGH);
    delay(1000);
    digitalWrite(LED_R, LOW);
    last_pir_trigger = millis();
    return true;
  }
  return false;
}


//---------Temperature/Humidity Sensor Functions---------//


bool th_init() {
  // initialize DHT20 sensor
  Wire.begin();
  if (!dht20.begin()) {
    Serial.println("Failed to find DHT20 sensor");
    return false;
  }
  return true;
}

bool th_read(Data * data) {
  if (millis() - dht20.lastRead() > 2000) {
    dht20.read();
    float hum = dht20.getHumidity();
    float temp = dht20.getTemperature();

    if (data->curr_hum != hum || data->curr_temp != temp) {
      data->curr_hum = hum;
      data->curr_temp = temp;
    }

    return true;
  }
  return false;
}

inline void th_print(Data * data) {
  Serial.print("Temperature: ");
  Serial.print(data->curr_temp, 1);
  Serial.println("°");
  Serial.print("Humidity: ");
  Serial.print(data->curr_hum, 1);
  Serial.println("%");
  Serial.println("---------------------");
}