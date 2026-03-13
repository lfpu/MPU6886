# MPU6886

## Features
## Supported platforms
## Installation (PlatformIO)
## Example



# MPU6886 — 获取与设置传感器数据

本仓库提供对 MPU6886（3 轴加速度 + 3 轴陀螺 + 温度）的寄存器级读取与简单换算示例，基于 I2C 通信。适用于 Arduino / PlatformIO 等常见开发环境。

主要功能：
- I2C 扫描与设备识别（WHO_AM_I）
- 连续读取 14 字节输出（Accel 6 + Temp 2 + Gyro 6）
- 将原始值转换为工程单位：g、°/s、℃（带换算公式）
- CSV 风格串口输出，便于记录和分析

## 目录
- 快速开始
- 硬件接线
- 示例代码
- 数据换算（公式）
- API 简要
- PlatformIO 使用
- 常见问题与排查
- 贡献与许可

## 快速开始

1. 将 MPU6886 接好电源与 I2C（SDA/SCL）。
2. 在工程中包含库并上传示例代码。
3. 使用串口监视器查看 CSV 输出：
   time(ms), ax(g), ay(g), az(g), gx(°/s), gy(°/s), gz(°/s), temp(°C)。

建议串口波特率：115200。

## 硬件接线（示例）
- VCC → 3.3V（或板子支持的 Vcc，请勿超出芯片额定）
- GND → GND
- SDA → 板子 SDA（或指定的 I2C_SDA）
- SCL → 板子 SCL（或指定的 I2C_SCL）

> 注意：ESP32 等需要在 `Wire.begin(SDA, SCL)` 指定引脚，UNO/Nano 等通常使用默认 I2C 引脚。

## 示例代码（可直接复制为 example）

```cpp
void setup() {
  Serial.begin(115200);
  delay(100);

  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(400000);

  scanI2C();

  IMU = MPU6886();

  uint8_t whoami = 0;
  if (IMU.readRegs(REG_WHO_AM_I, &whoami, 1)) {
    Serial.print("WHO_AM_I = 0x");
    Serial.println(whoami, HEX);
    if (whoami != WHO_AM_I_EXPECTED) {
      Serial.println("Warning: WHO_AM_I mismatch!");
    }
  } else {
    Serial.println("Read WHO_AM_I failed!");
  }
}
```
说明：请将示例中的占位符替换为你库定义：

- I2C_SDA / I2C_SCL：实际 I2C 引脚常量或数值
- REG_WHO_AM_I、REG_ACCEL_XOUT_H：寄存器地址常量
- ACCEL_SENS、GYRO_SENS：灵敏度常量
- WHO_AM_I_EXPECTED：WHO_AM_I 的期望值（例如 0x19；以实际芯片或模块为准）


# 数据换算（公式与变量说明）

变量与单位：

- a_raw：加速度原始值（LSB，int16）
- ω_raw：陀螺原始值（LSB，int16）
- temp_raw：温度原始值（int16）
- ACCEL_SENS：加速度灵敏度（LSB/g）
- GYRO_SENS：陀螺灵敏度（LSB/(°/s)）

# 常见问题与排查

- I2C scan 没找到设备：检查电源、SDA/SCL、上拉电阻与引脚配置。
- WHO_AM_I 值错误：确认寄存器地址与期望值。
- 数据噪声：检查量程设置与灵敏度常量。
- 串口卡顿：提高波特率或增大输出间隔。

# 输出样例（CSV）

 time(ms), ax(g), ay(g), az(g), gx(°/s), gy(°/s), gz(°/s), temp(°C)

 12345,0.0123,-0.0010,0.9998,0.45,-0.12,0.03,26.32