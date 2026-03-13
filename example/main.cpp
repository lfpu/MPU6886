#include <Arduino.h>
#include "MPU6886.h"


MPU6886 IMU;
void scanI2C()
{
  Serial.println("I2C scan...");
  for (uint8_t addr = 1; addr < 127; addr++)
  {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0)
    {
      Serial.print("Found device at 0x");
      if (addr < 16)
        Serial.print("0");
      Serial.println(addr, HEX);
    }
  }
}

void setup()
{
  // put your setup code here, to run once:

  Serial.begin(9600);

  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(400000); // MPU6886 支持 400kHz I2C

   scanI2C();
  IMU = MPU6886();
  uint8_t whoami = 0;
  if (IMU.readRegs(REG_WHO_AM_I, &whoami, 1))
  {
    Serial.print("WHO_AM_I = 0x");
    Serial.println(whoami, HEX);
  }
  else
  {
    Serial.println("Read WHO_AM_I failed!");
  }



  // Serial.println("MPU6886 init ok.");
}

void loop()
{


  uint8_t rawData[14];

  // 连续读取 14 字节：Accel(6) + Temp(2) + Gyro(6)
  if (IMU.readRegs(REG_ACCEL_XOUT_H, rawData, 14)) {
    int16_t axRaw = IMU.makeInt16(rawData[0], rawData[1]);
    int16_t ayRaw = IMU.makeInt16(rawData[2], rawData[3]);
    int16_t azRaw = IMU.makeInt16(rawData[4], rawData[5]);

    int16_t tempRaw = IMU.makeInt16(rawData[6], rawData[7]);

    int16_t gxRaw = IMU.makeInt16(rawData[8], rawData[9]);
    int16_t gyRaw = IMU.makeInt16(rawData[10], rawData[11]);
    int16_t gzRaw = IMU.makeInt16(rawData[12], rawData[13]);

    // 转成工程单位
    float ax_g = axRaw / ACCEL_SENS;
    float ay_g = ayRaw / ACCEL_SENS;
    float az_g = azRaw / ACCEL_SENS;

    float gx_dps = gxRaw / GYRO_SENS;
    float gy_dps = gyRaw / GYRO_SENS;
    float gz_dps = gzRaw / GYRO_SENS;

    float temp_c = tempRaw / 326.8f + 25.0f;

    Serial.print(millis());Serial.print(",");
    Serial.print(ax_g,4);Serial.print(",");
    Serial.print(ay_g,4);Serial.print(",");
    Serial.print(az_g,4);Serial.print(",");
    Serial.print(gx_dps, 4); Serial.print(",");
    Serial.print(gy_dps, 4); Serial.print(",");
    Serial.print(gz_dps, 4);Serial.print(",");
    Serial.println(temp_c, 2);
  } else {
    Serial.println("Read sensor data failed!");
  }

  delay(1);
}
