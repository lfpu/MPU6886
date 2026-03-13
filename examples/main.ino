#include <Arduino.h>
#include "MPU6886.h"


MPU6886 IMU;

void setup()
{
  // put your setup code here, to run once:

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
  float ax;
  float ay;
  float az;
  float gx;
  float gy;
  float gz;
  float t;
void loop()
{
  IMU.getDatas(&ax,&ay,&az,&gx,&gy,&gz,&t);
    Serial.print(millis());Serial.print(",");
    Serial.print(ax,4);Serial.print(",");
    Serial.print(ay,4);Serial.print(",");
    Serial.print(az,4);Serial.print(",");
    Serial.print(gx, 4); Serial.print(",");
    Serial.print(gy, 4); Serial.print(",");
    Serial.print(gz, 4);Serial.print(",");
    Serial.println(t, 2);
  delay(1);
}
