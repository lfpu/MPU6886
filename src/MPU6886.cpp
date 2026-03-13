#include "MPU6886.h"
#include <Arduino.h>

MPU6886::MPU6886()
{
    IsAvalible = Init();
}

MPU6886::~MPU6886()
{
}
bool MPU6886::writeReg(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(MPU6886_ADDR);
    Wire.write(reg);
    Wire.write(value);
    return (Wire.endTransmission() == 0);
}

bool MPU6886::readReg(uint8_t reg, uint8_t &value)
{
    Wire.beginTransmission(MPU6886_ADDR);
    Wire.write(reg);
    // 关键差异：这里不要用 endTransmission(false)
    if (Wire.endTransmission() != 0)
    {
        return false;
    }

    if (Wire.requestFrom((int)MPU6886_ADDR, 1) != 1)
    {
        return false;
    }

    value = Wire.read();
    return true;
}

bool MPU6886::readRegs(uint8_t reg, uint8_t *buf, size_t len)
{
    Wire.beginTransmission(MPU6886_ADDR);
    Wire.write(reg);

    if (Wire.endTransmission() != 0)
    {
        return false;
    }

    if (Wire.requestFrom((int)MPU6886_ADDR, (int)len) != (int)len)
    {
        return false;
    }

    for (size_t i = 0; i < len; ++i)
    {
        buf[i] = Wire.read();
    }
    return true;
}

int16_t MPU6886::makeInt16(uint8_t highByte, uint8_t lowByte)
{
    return (int16_t)((highByte << 8) | lowByte);
}

bool MPU6886::Init()
{
    uint8_t who = 0;
    if (!readReg(REG_WHO_AM_I, who))
    {
        Serial.println(who);
        Serial.println("[ERR] read WHO_AM_I failed");
        return false;
    }

    Serial.print("[INFO] WHO_AM_I = 0x");
    Serial.println(who, HEX);

    // MPU6886 的 WHO_AM_I 期望值通常为 0x19
    if (who != 0x19)
    {
        Serial.println("[ERR] WHO_AM_I is not 0x19, IMU may be wrong/not connected");
        return false;
    }

    uint8_t pwr = 0;
    if (readReg(REG_PWR_MGMT_1, pwr))
    {
        Serial.print("[INFO] PWR_MGMT_1 before init = 0x");
        Serial.println(pwr, HEX);
    }

    // 1) 软复位
    if (!writeReg(REG_PWR_MGMT_1, 0x80))
    {
        Serial.println("[ERR] write reset failed");
        return false;
    }
    delay(100);

    if (readReg(REG_PWR_MGMT_1, pwr))
    {
        Serial.print("[INFO] PWR_MGMT_1 after reset = 0x");
        Serial.println(pwr, HEX);
    }

    // 2) 清睡眠，先全开
    if (!writeReg(REG_PWR_MGMT_1, 0x00))
    {
        Serial.println("[ERR] clear sleep failed");
        return false;
    }
    delay(10);

    // 3) 选择更稳定的时钟源
    if (!writeReg(REG_PWR_MGMT_1, 0x01))
    {
        Serial.println("[ERR] set clock source failed");
        return false;
    }
    delay(10);

    if (!readReg(REG_PWR_MGMT_1, pwr))
    {
        Serial.println("[ERR] read back PWR_MGMT_1 failed");
        return false;
    }

    Serial.print("[INFO] PWR_MGMT_1 after wake = 0x");
    Serial.println(pwr, HEX);

    if (pwr != 0x01)
    {
        Serial.println("[ERR] wake failed: PWR_MGMT_1 is not 0x01");
        return false;
    }

    // 4) 使能所有轴
    if (!writeReg(REG_PWR_MGMT_2, 0x00))
    {
        Serial.println("[ERR] write PWR_MGMT_2 failed");
        return false;
    }
    delay(10);

    // 5) 配置量程/滤波（参考现成驱动常见做法）
    if (!writeReg(REG_ACCEL_CONFIG, 0x10))
    { // ±8g
        Serial.println("[ERR] write ACCEL_CONFIG failed");
        return false;
    }

    if (!writeReg(REG_GYRO_CONFIG, 0x00))
    { // ±250dps
        Serial.println("[ERR] write GYRO_CONFIG failed");
        return false;
    }

    if (!writeReg(REG_CONFIG, 0x03))
    {
        Serial.println("[ERR] write CONFIG failed");
        return false;
    }

    if (!writeReg(REG_SMPLRT_DIV, 0x09))
    {
        Serial.println("[ERR] write SMPLRT_DIV failed");
        return false;
    }

    if (!writeReg(REG_ACCEL_CONFIG2, 0x03))
    {
        Serial.println("[ERR] write ACCEL_CONFIG2 failed");
        return false;
    }

    Serial.println("[OK] MPU6886 init success");
    return true;
}
