#include <th.h>

//temp/hum sensor object
DHT20 dht20(&Wire);

void thInit() {
  //initialize DHT20 sensor
  Wire.begin();
  if (!dht20.begin()) {
    Serial.println("Failed to find DHT20 sensor");
  }
}

void readTH(unsigned long currentMillis) {
  if (currentMillis - lastSensorRead > 2000) {
    float humidity = dht20.getHumidity(), temp = dht20.getTemperature();

    if (currentHum != humidity || currentTemp != temp) {
      currentHum = humidity;
      currentTemp = temp;
      //updateScreen();

      /*if (deviceConnected) {
        pTempChar->setValue(String(currentTemp, 1).c_str());
        pTempChar->notify();
        pHumChar->setValue(String(currentHum, 1).c_str());
        pHumChar->notify();
      }*/
    }
      lastSensorRead = currentMillis;
  }
}

void printTHData(uint8_t temp, uint8_t humidity) {
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println("°");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
  Serial.println("---------------------");
}