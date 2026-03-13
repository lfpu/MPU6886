#include <Wire.h>

// ===== 根据你的板子修改 =====
#define I2C_SDA 43 // AtomS3: GPIO38
#define I2C_SCL 44 // AtomS3: GPIO39
#define MPU6886_ADDR 0x68

// ===== MPU6886 寄存器 =====
#define REG_WHO_AM_I 0x75
#define REG_PWR_MGMT_1 0x6B
#define REG_PWR_MGMT_2 0x6C
#define REG_SMPLRT_DIV 0x19
#define REG_CONFIG 0x1A
#define REG_GYRO_CONFIG 0x1B
#define REG_ACCEL_CONFIG 0x1C
#define REG_ACCEL_CONFIG2 0x1D

#define REG_ACCEL_XOUT_H 0x3B
#define REG_TEMP_OUT_H 0x41
#define REG_GYRO_XOUT_H 0x43

// ===== 灵敏度（当前代码设置为 ±8g, ±250dps）=====
static constexpr float ACCEL_SENS = 3730.0f; // LSB/g  配置量程/+-8g
static constexpr float GYRO_SENS = 131.0f;    // LSB/(deg/s)

class MPU6886
{
private:
    /* data */
    bool Init();

public:
    MPU6886();
    ~MPU6886();
    bool IsAvalible;
    bool writeReg(uint8_t reg, uint8_t value);
    bool readRegs(uint8_t reg, uint8_t *buf, size_t len);
    bool readReg(uint8_t reg, uint8_t &value);
    int16_t makeInt16(uint8_t highByte, uint8_t lowByte);
};
