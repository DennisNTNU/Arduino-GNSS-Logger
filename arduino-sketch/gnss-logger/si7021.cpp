
#include "si7021.hpp"
#include <Arduino.h>
#include <Wire.h>

int readDataSi7021(float* buf)
{
  Wire.beginTransmission(SI7021_ADDRESS);
  Wire.write(CMD_MEASURE_HUMIDITY);
  int ret = Wire.endTransmission(false);

  delay(20); // wait for humidity conversion

  int data_count = 0;
  byte humidity_raw[3] = {1, 2, 3};
  byte temperature_raw[3] = {1, 2, 3};

  byte data_received = Wire.requestFrom(SI7021_ADDRESS, 3);
  while (Wire.available() > 0)
  {
    humidity_raw[data_count] = Wire.read();
    data_count++;
  }
  
  Wire.beginTransmission(SI7021_ADDRESS);
  Wire.write(CMD_READ_TEMP_CMD);
  ret = Wire.endTransmission(false);

  data_count = 0;
  data_received = Wire.requestFrom(SI7021_ADDRESS, 3);
  while (Wire.available() > 0)
  {
    temperature_raw[data_count] = Wire.read();
    data_count++;
  }

  uint16_t hum = humidity_raw[0];
  hum = hum << 8 | humidity_raw[1];
  // uint8_t chxsum = humidity_raw[2];
  float humidity = hum;
  humidity *= 125;
  humidity /= 65536;
  humidity -= 6;

  uint16_t temp = temperature_raw[0];
  temp = (temp << 8 | temperature_raw[1]);
  // uint8_t chxsum = humidity_raw[2];
  float temperature = temp;
  temperature = 175.25*temperature;
  temperature = temperature/65536.0 - 46.85;

  buf[0] = humidity;
  buf[1] = temperature;

  return 0;
}
