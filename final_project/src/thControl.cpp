#include <../include/thControl.h>

void thInit() {
  Wire.begin();
  writeToTH(0x71);
  if ((readByteFromTH() & 0x18) != 0x18) {
    Serial.println("TH Initialization Failed");
    while(1) {};
  }
}

void writeToTH(uint8_t msg) {
  Wire.beginTransmission(TH_ADDR);
  Wire.write(msg);
  Wire.endTransmission();
}

uint8_t readByteFromTH() {
  Wire.requestFrom(TH_ADDR, 1);
  return Wire.read();
}

void readFromTH(uint8_t * data) {
  Serial.println("Reading...");
  Serial.println();
  delay(10);
  writeToTH(0xAC);
  writeToTH(0x33);
  writeToTH(0x00);
  
  delay(80);
  while(1) {
    if (!(readByteFromTH() >> 7)) { break; }
  }
  uint8_t data[6];
  for (int i = 0; i < 6; i++) {
    data[i] = readByteFromTH();
  }
}

uint8_t calcHumidity(uint8_t * data) {
  //takes a buffer of temp/hum data and runs the humidiity calculation
  uint32_t sRH = data[0] << 12;
  sRH = sRH | (data[1] << 4);
  sRH = sRH | (data[2] >> 4);
  return (sRH / (2 ^ 20)) * 100;
}

uint8_t calcTemp(uint8_t * data) {
  //takes a buffer of temp/hum data and runs the temperature calculation
  uint32_t sT = data[0] << 12;
  sT = sT | (data[1] << 4);
  sT = sT | (data[2] >> 4);
  return (sT / (2^20)) * 200 - 50;
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



/*
void setup() {
  thInit();
  Serial.begin(9600);
  I2CScan();
}

void loop() {
  uint8_t data[6];
  readFromTH(data);
  printTHData(calcTemp(data), calcHumidity(data));
  delay(1000);
}
*/

